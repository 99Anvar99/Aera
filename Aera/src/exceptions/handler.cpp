#include "handler.h"
#include "minhook/hde/hde64.h"
#include "util/util.h"

namespace exceptions
{
	u8 get_instruction_length(u8* code)
	{
		u8* start{code};
		hde64s hde{};
		if (u8 len{static_cast<u8>(hde64_disasm(code, &hde))})
		{
			return len;
		}
		if (x64::disassemble_instruction_code(code).is_valid())
		{
			return static_cast<u8>(code - start);
		}
		return NULL;
	}

	bool attempt_stack_recovery(PEXCEPTION_POINTERS exception_info)
	{
		PCONTEXT ctx{exception_info->ContextRecord};
		g_recovery_count++;
		if (g_recovery_count >= 5)
			g_running = false;
		if (const u8 len{get_instruction_length(reinterpret_cast<u8*>(ctx->Rip))})
		{
			ctx->Rip += len;
			return true;
		}
		return false;
	}

	std::string get_exception_type(ULONG_PTR type)
	{
		if (g_exception_error_reasons[type]())
		{
			return g_exception_error_reasons[type].what();
		}
		return "unknown";
	}

	LONG CALLBACK on_exception_callback(PEXCEPTION_POINTERS exceptionInfo)
	{
		LOG(Trace, "Game base address: 0x{:X}", reinterpret_cast<u64>(GetModuleHandleA(NULL)))
		stackWalker sw(StackWalker::AfterExcept);
		exceptionContext ctx{exceptionInfo};
		if (!ctx.m_fileoffset.empty())
		{
			std::string name{
				g_exception_types[ctx.m_code]()
					? g_exception_types[ctx.m_code].str()
					: std::format("0x{:X}", ctx.m_code)
			};
			switch (ctx.m_code)
			{
			case CONTROL_C_EXIT:
			case EXCEPTION_BREAKPOINT:
			case EXCEPTION_SINGLE_STEP:
				{
					LOG(Trace, "The game has suffered a non-fatal exception, you may disregard this message ({} at {})",
					    name, ctx.m_fileoffset)
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				break;
			case EXCEPTION_GUARD_PAGE:
			case EXCEPTION_ACCESS_VIOLATION:
				{
					std::string type{get_exception_type(ctx.m_type)};
					LOG(Trace,
					    "The game suffered an fatal exception, you may need to restart the game. ({} at {}, reason of {} was {}{})",
					    name, ctx.m_fileoffset, name, type,
					    ctx.m_type != 8 && name != "unknown" ? "" : std::format("0x{:X}", ctx.m_deathAddress))
				}
				break;
			default:
				{
					LOG(Trace,
					    "The game suffered a exception of unknown severity, you may need to restart the game. ({} at {}, reason of exception is unknown)",
					    name, ctx.m_fileoffset)
				}
				break;
			}
		}
		LOG(Trace, "Dumping registers...")
		ctx.print_registers();
		LOG(Trace, "Showing callstack...")
		sw.ShowCallstack(GetCurrentThread(), &ctx.m_context);
		if (attempt_stack_recovery(exceptionInfo))
		{
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	bool on_exception_callback_try_except(PEXCEPTION_POINTERS exception_info)
	{
		if (on_exception_callback(exception_info) != EXCEPTION_CONTINUE_EXECUTION)
			return false;
		return true;
	}

	LONG CALLBACK exception_handler(PEXCEPTION_POINTERS exception_info)
	{
		return on_exception_callback(exception_info);
	}

	LONG CALLBACK unhandled_exception_handler(PEXCEPTION_POINTERS exception_info)
	{
		return on_exception_callback(exception_info);
	}

	void init_exception_handler()
	{
		g_handler = AddVectoredExceptionHandler(TRUE, exception_handler);
		SetUnhandledExceptionFilter(unhandled_exception_handler);
	}

	void uninit_exception_handler()
	{
		if (g_handler)
		{
			RemoveVectoredExceptionHandler(g_handler);
		}
	}
}
