#pragma once
#include "pch/pch.h"

namespace util
{
	constexpr std::string_view g_base64Charaters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

	constexpr bool isCharacterBase64(u8 c)
	{
		return g_base64Charaters.find(c) != std::string_view::npos;
	}

	inline std::string base64Encode(const std::string& str)
	{
		u8 strLen{static_cast<u8>(str.length())};
		u8 i{}, j{}, in_{};
		u8 charArray4[4], charArray3[3];
		std::string ret{};
		while (strLen--)
		{
			charArray3[i++] = str[in_];
			in_++;
			if (i == 3)
			{
				charArray4[0] = (charArray3[0] & 0xFC) >> 2;
				charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xF0) >> 4);
				charArray4[2] = ((charArray3[1] & 0x0F) << 2) + ((charArray3[2] & 0xC0) >> 6);
				charArray4[3] = charArray3[2] & 0x3F;
				for (i = 0; (i < 4); i++)
					ret += g_base64Charaters[charArray4[i]];
				i = 0;
			}
		}
		if (i)
		{
			for (j = i; j < 3; j++)
				charArray3[j] = '\0';
			charArray4[0] = (charArray3[0] & 0xFC) >> 2;
			charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xF0) >> 4);
			charArray4[2] = ((charArray3[1] & 0x0F) << 2) + ((charArray3[2] & 0xC0) >> 6);
			charArray4[3] = charArray3[2] & 0x3f;
			for (j = 0; (j < i + 1); j++)
				ret += g_base64Charaters[charArray4[j]];
			while ((i++ < 3))
				ret += '=';
		}
		return ret;
	}

	inline std::string base64Decode(const std::string& str)
	{
		u8 strLen{static_cast<u8>(str.length())};
		u8 i{}, j{}, in_{};
		u8 charArray4[4], charArray3[3];
		std::string ret{};
		while (strLen-- && (str[in_] != '=') && isCharacterBase64(str[in_]))
		{
			charArray4[i++] = str[in_];
			in_++;
			if (i == 4)
			{
				for (i = 0; i != 4; ++i)
					charArray4[i] = isCharacterBase64(charArray4[i]);
				charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
				charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
				charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];
				for (i = 0; (i < 3); i++)
					ret += charArray3[i];
				i = 0;
			}
		}
		if (i)
		{
			for (j = i; j != 4; ++j)
				charArray4[j] = 0;
			for (j = 0; j != 4; ++j)
				charArray4[j] = isCharacterBase64(charArray4[j]);
			charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
			charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
			charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];
			for (j = 0; (j != i - 1); ++j)
				ret += charArray3[j];
		}
		return ret;
	}
}
