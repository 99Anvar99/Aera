#pragma once
#include "rage/commands/conversion_table.h"
#include "rage/classes.h"

class custom_call_context : public rage::scrNativeCallContext
{
public:
	constexpr custom_call_context()
	{
		m_return_value = &m_return_stack[0];
		m_args = &m_arg_stack[0];
	}

private:
	uint64_t m_return_stack[10];
	uint64_t m_arg_stack[40];
};

class native_invoker
{
	static inline rage::scrNativeHandler m_handlers[g_crossmap.size()];
	static inline bool m_are_handlers_cached{false};

public:
	constexpr native_invoker()
	{
	}

	constexpr void begin_call()
	{
		m_call_context.reset();
	}

	template <int Index, bool FixVectors>
	constexpr void end_call()
	{
		if (!m_are_handlers_cached)
			cache_handlers();

		m_handlers[Index](&m_call_context);
		if constexpr (FixVectors)
			this->fix_vectors();
	}

	template <typename T>
	constexpr void push_arg(T&& value)
	{
		m_call_context.push_arg(std::forward<T>(value));
	}

	template <typename T>
	constexpr T& get_return_value()
	{
		return *m_call_context.get_return_value<T>();
	}

	void fix_vectors();

	static void __declspec(noinline) cache_handlers();

	static rage::scrNativeHandler* get_handlers()
	{
		if (!m_are_handlers_cached)
			cache_handlers();

		return m_handlers;
	}

	template <int Index, bool FixVectors, typename Ret, typename... Args>
	static constexpr FORCEINLINE Ret invoke(Args&&... args)
	{
		native_invoker invoker{};

		invoker.begin_call();
		(invoker.push_arg(std::forward<Args>(args)), ...);
		invoker.end_call<Index, FixVectors>();

		if constexpr (!std::is_same_v<Ret, void>)
		{
			return invoker.get_return_value<Ret>();
		}
	}

	custom_call_context m_call_context{};
};