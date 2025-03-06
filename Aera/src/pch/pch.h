#pragma once
#define CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")

#include "framework.h"
#include "rage/enums.h"
#define BRAND "Aera"
#define UPPER_BRAND "AERA"
#define LOWER_BRAND "aera"
#define DEBUG

#define SIZEOF(a) sizeof(a) / sizeof(std::remove_pointer_t<decltype(a)>)
#define COUNT(a) ((sizeof(a)/sizeof(0[a])) / ((size_t)(!(sizeof(a) % sizeof(0[a])))))
#define ONCE_PER_FRAME(a) do a while (false)
#define ONCE(v, a) static bool v{ ([&] { a }(), true) };
constexpr long double pi{3.141592653589793238462643383279502884L};

template <typename T>
using comPtr = Microsoft::WRL::ComPtr<T>;
template <typename T> requires std::is_function_v<T>
using fnptr = std::add_pointer_t<T>;

template <typename Invokable, typename... InvokableArgs>
concept is_invokable_with_args = requires(Invokable callable, const InvokableArgs&... va_args)
{
	callable(va_args...);
};


class stackWalker : public StackWalker
{
public:
	stackWalker()
	{
	}

	explicit stackWalker(ExceptType exType) : StackWalker(exType)
	{
	}

	void OnOutput(LPCSTR sz_text) override;
	void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType,
	                  LPCSTR pdbName, ULONGLONG fileVersion) override;
	void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName) override;
	void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override;
	void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override;
};

namespace defines
{
	inline bool g_running{true};
	inline HMODULE g_module{};
	inline HANDLE g_thread{};
	inline LPDWORD g_main_thread{};

	inline bool is_number(const std::string& str)
	{
		return std::ranges::all_of(str, [](const char c) { return std::isdigit(c); });
	}

	inline bool contains_an_number(const std::string& str)
	{
		return std::ranges::any_of(str, [](const char c) { return std::isdigit(c); });
	}

	inline std::string string_to_lower(const std::string& str)
	{
		std::string result{str};
		std::ranges::transform(result, result.data(), [](const char c) { return tolower(c); });
		return result;
	}

	inline std::string string_to_upper(const std::string& str)
	{
		std::string result{str};
		std::ranges::transform(result, result.data(), [](const char c) { return toupper(c); });
		return result;
	}

	inline std::vector<std::string> get_matches(std::string str, const std::string& ex)
	{
		std::vector<std::string> matches{};
		const std::regex expression{ex};
		std::sregex_iterator iter{str.begin(), str.end(), expression};
		const std::sregex_iterator end{};
		while (iter != end)
		{
			matches.push_back(iter->str());
			++iter;
		}
		return matches;
	}

	inline std::vector<u64> find_all_occurrences(const std::string& str, const std::string& substr)
	{
		std::vector<u64> indexes{};
		u64 index{};
		while ((index = str.find(substr, index)) != std::string::npos)
		{
			indexes.push_back(index);
			index += substr.length();
		}
		return indexes;
	}

	inline std::string get_file_contents(const std::filesystem::path& path)
	{
		std::ifstream file{path};
		return {(std::istreambuf_iterator(file)), std::istreambuf_iterator<char>()};
	}

	inline std::string trim_string(std::string string, const char character)
	{
		string.erase(std::ranges::remove(string, character).begin(), string.end());
		return string;
	}

	template <typename T>
	float to_fixed(T number, int amount)
	{
		float multiplier = std::pow(10.0f, static_cast<float>(amount));
		return std::round(number * multiplier) / multiplier;
	}

	inline std::string remove_zeros(float number)
	{
		std::string str = std::to_string(number);
		str.erase(str.find_last_not_of('0') + 1, std::string::npos);
		str.erase(str.find_last_not_of('.') + 1, std::string::npos);
		return str;
	}

	inline std::vector<std::string> split_string(const std::string& string, const char split)
	{
		std::vector<std::string> output{};
		size_t previous_position{};
		size_t position{string.find(split)};
		while (position != std::string::npos)
		{
			std::string substring{string.substr(previous_position, position - previous_position)};
			output.push_back(substring);
			previous_position = position + 1;
			position = string.find(split, previous_position);
		}
		const std::string last_substring{string.substr(previous_position)};
		output.push_back(last_substring);
		return output;
	}

	// Function to check if a vector contains a string
	inline bool contains_string(std::vector<std::string> vec, const std::string& str)
	{
		return std::ranges::find(vec, str) != vec.end();
	}

	// Function to check if a string is an integer
	inline bool is_integer(const std::string& str)
	{
		try
		{
			int integer_value = std::stoi(str); // Attempt to convert the string to an integer
			return true;
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}

	// Function to check if a string is a floating-point number
	inline bool is_float(const std::string& str)
	{
		try
		{
			float float_value = std::stof(str); // Attempt to convert the string to a float
			return true;
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
		catch (...)
		{
			return false;
		}
	}

	inline bool contains_a_character(const std::string& str)
	{
		return std::ranges::any_of(str, [](const char c) { return std::isalpha(c); });
	}
}

using namespace defines;

namespace fs = std::filesystem;
