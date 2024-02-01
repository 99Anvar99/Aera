#pragma once
#include "pch/pch.h"
#include "core/logger.h"
#include "disassembler.h"

namespace exceptions
{
	class exceptionError
	{
	public:
		exceptionError(i32 t, std::string_view r) : m_type(t), m_reason(r)
		{
		}

		std::string what()
		{
			return m_reason.data();
		}

		int type()
		{
			return m_type;
		}

		bool operator()()
		{
			return !what().empty();
		}

	protected:
		i32 m_type{};
		std::string_view m_reason{};
	};

	class exceptionType
	{
	public:
		exceptionType(u32 code, cc* str) : m_code(code), m_string(str)
		{
		}

		std::string str()
		{
			return m_string.data();
		}

		uint32_t code()
		{
			return m_code;
		}

		bool operator()()
		{
			return !str().empty();
		}

	protected:
		u32 m_code{};
		std::string_view m_string{};
	};

	class exceptionContext
	{
	public:
		exceptionContext(EXCEPTION_POINTERS* exception)
			: m_record(*exception->ExceptionRecord), m_context(*exception->ContextRecord),
			  m_type(m_record.ExceptionInformation[0]), m_deathAddress(m_record.ExceptionInformation[1])
		{
			m_code = m_record.ExceptionCode;
			m_exceptionAddress = reinterpret_cast<uint64_t>(m_record.ExceptionAddress);
		}

		void print_registers()
		{
			LOG(Trace, "RAX: 0x{:X} || RSI: 0x{:X}", m_context.Rax, m_context.Rsi)
			LOG(Trace, "RBX: 0x{:X} || RDI: 0x{:X}", m_context.Rbx, m_context.Rdi)
			LOG(Trace, "RCX: 0x{:X} || RBP: 0x{:X}", m_context.Rcx, m_context.Rbp)
			LOG(Trace, "RDX: 0x{:X} || RSP: 0x{:X}", m_context.Rdx, m_context.Rsp)
			LOG(Trace, "R8: 0x{:X}  || R9: 0x{:X}", m_context.R8, m_context.R9)
			LOG(Trace, "R10: 0x{:X} || R11: 0x{:X}", m_context.R10, m_context.R11)
			LOG(Trace, "R12: 0x{:X} || R13: 0x{:X}", m_context.R12, m_context.R13)
			LOG(Trace, "R14: 0x{:X} || R15: 0x{:X}", m_context.R14, m_context.R15)
		}

		u32 m_code{};
		u64 m_exceptionAddress{};
		EXCEPTION_RECORD& m_record;
		CONTEXT& m_context;
		u64 m_type{};
		u64 m_deathAddress{};
		std::string m_fileoffset{};
	};

	inline u64 g_recovery_count{};
	extern bool attempt_stack_recovery(PEXCEPTION_POINTERS exception_info);
#define PAIR(v) exceptionType(v, #v)
	inline std::array g_exception_types{
		PAIR(STATUS_WAIT_0),
		PAIR(STATUS_ABANDONED_WAIT_0),
		PAIR(STATUS_USER_APC),
		PAIR(STATUS_TIMEOUT),
		PAIR(STATUS_PENDING),
		PAIR(DBG_EXCEPTION_HANDLED),
		PAIR(DBG_CONTINUE),
		PAIR(STATUS_SEGMENT_NOTIFICATION),
		PAIR(STATUS_FATAL_APP_EXIT),
		PAIR(DBG_REPLY_LATER),
		PAIR(DBG_TERMINATE_THREAD),
		PAIR(DBG_TERMINATE_PROCESS),
		PAIR(DBG_CONTROL_C),
		PAIR(DBG_PRINTEXCEPTION_C),
		PAIR(DBG_RIPEXCEPTION),
		PAIR(DBG_CONTROL_BREAK),
		PAIR(DBG_PRINTEXCEPTION_WIDE_C),
		PAIR(DBG_COMMAND_EXCEPTION),
		PAIR(STATUS_GUARD_PAGE_VIOLATION),
		PAIR(STATUS_DATATYPE_MISALIGNMENT),
		PAIR(STATUS_BREAKPOINT),
		PAIR(STATUS_SINGLE_STEP),
		PAIR(STATUS_LONGJUMP),
		PAIR(STATUS_UNWIND_CONSOLIDATE),
		PAIR(DBG_EXCEPTION_NOT_HANDLED),
		PAIR(STATUS_ACCESS_VIOLATION),
		PAIR(STATUS_IN_PAGE_ERROR),
		PAIR(STATUS_INVALID_HANDLE),
		PAIR(STATUS_INVALID_PARAMETER),
		PAIR(STATUS_NO_MEMORY),
		PAIR(STATUS_ILLEGAL_INSTRUCTION),
		PAIR(STATUS_NONCONTINUABLE_EXCEPTION),
		PAIR(STATUS_INVALID_DISPOSITION),
		PAIR(STATUS_ARRAY_BOUNDS_EXCEEDED),
		PAIR(STATUS_FLOAT_DENORMAL_OPERAND),
		PAIR(STATUS_FLOAT_DIVIDE_BY_ZERO),
		PAIR(STATUS_FLOAT_INVALID_OPERATION),
		PAIR(STATUS_FLOAT_OVERFLOW),
		PAIR(STATUS_FLOAT_STACK_CHECK),
		PAIR(STATUS_FLOAT_UNDERFLOW),
		PAIR(STATUS_INTEGER_DIVIDE_BY_ZERO),
		PAIR(STATUS_INTEGER_OVERFLOW),
		PAIR(STATUS_PRIVILEGED_INSTRUCTION),
		PAIR(STATUS_STACK_OVERFLOW),
		PAIR(STATUS_DLL_NOT_FOUND),
		PAIR(STATUS_ORDINAL_NOT_FOUND),
		PAIR(STATUS_ENTRYPOINT_NOT_FOUND),
		PAIR(STATUS_CONTROL_C_EXIT),
		PAIR(STATUS_DLL_INIT_FAILED),
		PAIR(STATUS_CONTROL_STACK_VIOLATION),
		PAIR(STATUS_FLOAT_MULTIPLE_FAULTS),
		PAIR(STATUS_REG_NAT_CONSUMPTION),
		PAIR(STATUS_HEAP_CORRUPTION),
		PAIR(STATUS_STACK_BUFFER_OVERRUN),
		PAIR(STATUS_INVALID_CRUNTIME_PARAMETER),
		PAIR(STATUS_ASSERTION_FAILURE),
		PAIR(STATUS_ENCLAVE_VIOLATION),
		PAIR(STATUS_INTERRUPTED),
		PAIR(STATUS_THREAD_NOT_RUNNING),
		PAIR(STATUS_ALREADY_REGISTERED),
		PAIR(STATUS_SXS_EARLY_DEACTIVATION),
		PAIR(STATUS_SXS_INVALID_DEACTIVATION)
	};
	inline std::array g_exception_error_reasons{
		exceptionError(0, "a attempt to read an invalid address"),
		exceptionError(1, "a attempt to write to an invalid address"),
		exceptionError(8, "an data exception prevention (DEP)")
	};
	extern std::string get_exception_type(ULONG_PTR type);
	extern LONG CALLBACK on_exception_callback(PEXCEPTION_POINTERS exceptionInfo);
	extern bool on_exception_callback_try_except(PEXCEPTION_POINTERS exception_info);
	extern LONG CALLBACK exception_handler(PEXCEPTION_POINTERS exception_info);
	extern LONG CALLBACK unhandled_exception_handler(PEXCEPTION_POINTERS exception_info);
	inline PVOID g_handler{};
	extern void init_exception_handler();
	extern void uninit_exception_handler();
}
