#pragma once
#include "pch/pch.h"
#include "mem.h"
#include "module.h"
#include "core/logger.h"

inline u16 g_foundSigCount{};
inline u16 g_totalSigCount{};
inline u16 g_failedSigCount{};

inline std::optional<u8> charToHex(cc c)
{
	if (c >= 'a' && c <= 'f')
		return static_cast<u8>(static_cast<i32>(c) - 87);
	if (c >= 'A' && c <= 'F')
		return static_cast<u8>(static_cast<i32>(c) - 55);
	if (c >= '0' && c <= '9')
		return static_cast<u8>(static_cast<i32>(c) - 48);
	return {};
}

inline std::vector<std::optional<u8>> createBytesFromString(const std::string& ptr)
{
	std::vector<std::optional<u8>> bytes{};
	for (size_t i{}; i != ptr.size() - 1; ++i)
	{
		if (ptr[i] == ' ')
			continue;
		if (ptr[i] != '?')
		{
			if (auto c = charToHex(ptr[i]); c)
			{
				if (auto c2 = charToHex(ptr[i + 1]); c2)
					bytes.emplace_back(static_cast<uint8_t>((*c * 0x10) + *c2));
			}
		}
		else
		{
			bytes.emplace_back();
		}
	}
	return bytes;
}

inline bool doesMemoryMatch(u8* target, const std::optional<u8>* sig, u64 len)
{
	for (u64 i{}; i != len; ++i)
	{
		if (sig[i] && *sig[i] != target[i])
		{
			return false;
		}
	}
	return true;
}

inline u64 findPatternBruteforce(const std::vector<std::optional<u8>>& bytes, hmodule module = {})
{
	for (u64 i{}; i != module.size() - bytes.size(); ++i)
	{
		if (doesMemoryMatch(module.begin().add(i).as<u8*>(), bytes.data(), bytes.size()))
		{
			return module.begin().as<u64>() + i;
		}
	}
	return NULL;
}

inline u64 findPatternBoyerMooreHorspool(const std::vector<std::optional<u8>>& bytes, hmodule module = {})
{
	u64 maxShift{bytes.size()};
	u64 maxIdx{maxShift - 1};
	//Get wildcard index, and store max shifable byte count
	u64 wildCardIdx{static_cast<u64>(-1)};
	for (i32 i{static_cast<i32>(maxIdx - 1)}; i >= 0; --i)
	{
		if (!bytes[i])
		{
			maxShift = maxIdx - i;
			wildCardIdx = i;
			break;
		}
	}
	//Store max shiftable bytes for non wildcards.
	u64 shiftTable[UINT8_MAX + 1]{};
	for (u64 i{}; i <= UINT8_MAX; ++i)
		shiftTable[i] = maxShift;
	for (u64 i{wildCardIdx + 1}; i != maxIdx; ++i)
		shiftTable[*bytes[i]] = maxIdx - i;
	//Loop data
	for (u64 curIdx{}; curIdx != module.size() - bytes.size();)
	{
		for (u64 sigIdx = maxIdx; sigIdx >= 0; --sigIdx)
		{
			if (bytes[sigIdx] && *module.begin().add(curIdx + sigIdx).as<u8*>() != *bytes[sigIdx])
			{
				curIdx += shiftTable[*module.begin().add(curIdx + maxIdx).as<u8*>()];
				break;
			}
			if (sigIdx == NULL)
			{
				return module.begin().add(curIdx).as<u64>();
			}
		}
	}
	return NULL;
}

class scanner
{
public:
	scanner(std::string name, std::string pattern, const hmodule& module = {}) :
		m_name(std::move(name)), m_pattern(std::move(pattern)), m_module(module), m_elements(createBytesFromString(m_pattern))
	{
	}

	mem get()
	{
		g_totalSigCount++;
		mem res{findPatternBoyerMooreHorspool(m_elements, m_module)};
		if (res)
		{
			g_foundSigCount++;
			LOG_DEBUG("Found {} at GTA5.exe+0x{:X}", m_name, res.as<u64>() - m_module.begin().as<u64>())
		}
		else
		{
			g_failedSigCount++;
			LOG(Info, "Failed to find {}", m_name)
		}
		return res;
	}

private:
	std::string m_name{};
	std::string m_pattern{};
	hmodule m_module{};
	std::vector<std::optional<u8>> m_elements{};
};

inline mem scan(const std::string& key, const std::string& ptr, const hmodule& module = {})
{
	return scanner(key, ptr, module).get();
}

inline std::vector<mem> getAllResults(const std::string& ptr, hmodule module = {})
{
	std::vector bytes{createBytesFromString(ptr)};
	std::vector<mem> results{};
	for (u64 i{}; i != module.size() - bytes.size(); ++i)
	{
		if (doesMemoryMatch(module.begin().add(i).as<u8*>(), bytes.data(), bytes.size()))
		{
			results.push_back(module.begin().add(i));
		}
	}
	return results;
}
