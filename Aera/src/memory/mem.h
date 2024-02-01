#pragma once
#include "pch/pch.h"

class mem
{
public:
	mem(void* p = {}) : m_ptr(p)
	{
	}

	mem(u64 p) : mem(reinterpret_cast<void*>(p))
	{
	}

	template <typename T>
	std::enable_if_t<std::is_pointer_v<T>, T> as()
	{
		return static_cast<T>(m_ptr);
	}

	template <typename T>
	std::enable_if_t<std::is_lvalue_reference_v<T>, T> as()
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(m_ptr);
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<T, uintptr_t>, T> as()
	{
		return reinterpret_cast<T>(m_ptr);
	}

	mem mov()
	{
		return add(3).rip();
	}

	mem cmp()
	{
		return add(3).rip();
	}

	mem lea()
	{
		return add(2).rip();
	}

	mem call()
	{
		return add(1).rip();
	}

	mem add(u64 v)
	{
		return {as<u64>() + v};
	}

	mem sub(u64 v)
	{
		return {as<u64>() - v};
	}

	mem rip()
	{
		return add(as<i32&>()).add(4);
	}

	operator bool()
	{
		return m_ptr;
	}

private:
	void* m_ptr{};
};
