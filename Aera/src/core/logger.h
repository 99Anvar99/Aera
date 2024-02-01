#pragma once
#include "pch/pch.h"
#define LOG_DIRECT(c, t, s, ...) g_logger.get()->send(eLogColor::c, t, s, __VA_ARGS__);
#define LOG(t, s, ...) g_logger.get()->send(eLogType::t, s, __VA_ARGS__);
#ifdef DEBUG
#define LOG_DEBUG(s, ...) LOG(Debug, s, __VA_ARGS__);
#elif
#define LOG_DEBUG(s, ...) ;
#endif

enum class eLogColor : u16
{
	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	Intensify = FOREGROUND_INTENSITY,
	White = Red | Green | Blue | Intensify,
	Grey = Intensify,
	LightRed = Red | Intensify,
	LightGreen = Green | Intensify,
	LightBlue = Blue | Intensify
};

inline eLogColor operator|(eLogColor a, eLogColor b)
{
	return static_cast<eLogColor>(static_cast<std::underlying_type_t<eLogColor>>(a) | static_cast<std::underlying_type_t
		<eLogColor>>(b));
}

enum class eLogType : u8
{
	Info,
	Warn,
	Fatal,
	Exception,
	Registers,
	Stackwalker,
	Trace,
	Commands,
	Session,
#ifdef DEBUG
	Debug
#endif
};

class logger
{
public:
	explicit logger(const std::string& title)
	{
		m_path /= BRAND;
		if (!exists(m_path))
			create_directory(m_path);
		m_path /= "Log.txt";
		m_file.open(m_path.string(), std::ios::out | std::ios::trunc);
		if (!AttachConsole(GetCurrentProcessId()))
			AllocConsole();
		SetConsoleTitleA(title.c_str());
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
		m_con.open("CONOUT$");
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	}

	~logger()
	{
		FreeConsole();
		fclose(stdout);
		m_path.clear();
		m_con.close();
		m_file.close();
	}

	template <typename... Arguments>
	void send(eLogColor color, const std::string& type, const std::string& fmt, Arguments... args)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<u16>(color));
		const std::string& msg{std::vformat(fmt, std::make_format_args(args...))};
		m_con << type << " | " << msg << "\n" << std::flush;
	        m_file << type << " | " << msg << "\n" << std::flush;
	}

	template <typename... Arguments>
	void send(const eLogType type, std::string fmt, Arguments... args)
	{
		switch (type)
		{
		case eLogType::Info:
			{
				send(eLogColor::Grey, "Info", fmt, args...);
			}
			break;
		case eLogType::Warn:
			{
				send(eLogColor::LightRed, "Warn", fmt, args...);
			}
			break;
		case eLogType::Fatal:
			{
				send(eLogColor::Red, "Fatal", fmt, args...);
			}
			break;
		case eLogType::Exception:
			{
				send(eLogColor::LightRed, "Exception", fmt, args...);
			}
			break;
		case eLogType::Trace:
			{
				send(eLogColor::Red, "Trace", fmt, args...);
			}
			break;
		case eLogType::Registers:
			{
				send(eLogColor::Grey, "Registers", fmt, args...);
			}
			break;
		case eLogType::Stackwalker:
			{
				send(eLogColor::LightRed, "StackWalker", fmt, args...);
			}
			break;
		case eLogType::Commands:
			{
				send(eLogColor::White, "Commands", fmt, args...);
			}
			break;
		case eLogType::Session:
			{
				send(eLogColor::White, "Session", fmt, args...);
			}
			break;
#ifdef DEBUG
		case eLogType::Debug:
			{
				send(eLogColor::White, "Debug", fmt, args...);
			}
			break;
#endif
		default:
			{
			}
			break;
		}
	}

private:
	fs::path m_path{std::getenv("appdata")};
	std::ofstream m_con{};
	std::ofstream m_file{};
};

inline std::unique_ptr<logger> g_logger{};
