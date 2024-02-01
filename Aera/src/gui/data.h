#pragma once
#include "pch/pch.h"
#include "rage/enums.h"
#define GET_U8(bitset, shift) (((bitset) >> (shift)) & 0xFF)

namespace ui
{
	class color
	{
	public:
		color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a)
		{
		}

		color(int r, int g, int b) : color(r, g, b, 255)
		{
		}

		color(u32 packed) : color(GET_U8(packed, 0), GET_U8(packed, 8), GET_U8(packed, 16), GET_U8(packed, 24))
		{
		}

		color()
		{
		}

		int r{};
		int g{};
		int b{};
		int a{};

		u32 pack()
		{
			return a << 24 | b << 16 | g << 8 | r << 0;
		}

		float* float4()
		{
			float v[4]{toFloat(a), toFloat(g), toFloat(b), toFloat(a)};
			return v;
		}

	private:
		static float toFloat(u8 v)
		{
			return (v & 0xFF) / 255.f;
		}

		static u8 fromFloat(float v)
		{
			return static_cast<u8>(v * 255.f);
		}
	};

	enum class eJustify : u8
	{
		Left,
		Right,
		Center
	};

	class timer
	{
	public:
		void start(u64 Time)
		{
			if (m_tick)
			{
				m_readyAt = GetTickCount64() + Time;
				m_tick = false;
			}
		}

		bool ready()
		{
			return GetTickCount64() > m_readyAt;
		}

		void reset()
		{
			m_tick = true;
		}

	private:
		u64 m_readyAt{};
		bool m_tick{};
	};

	class sprite
	{
	public:
		std::string m_dictionary{};
		std::string m_texture{};
		float m_rotation{};
	};

	class soundData
	{
	public:
		bool m_active{};
		std::string_view m_id{};
		std::string_view m_set{"HUD_FRONTEND_DEFAULT_SOUNDSET"};
	};

	class input
	{
	public:
		input(i8 key, u64 delay, eControl control, eControl secondaryControl, i8 keyGroup) :
			m_key(key), m_delay(delay),
			m_control(control), m_secondaryControl(secondaryControl), m_keyGroup(keyGroup)
		{
		}

		input(i8 key, u64 delay, eControl control, eControl secondaryControl) : input(
			key, delay, control, secondaryControl, 2)
		{
		}

		input(i8 key, u64 delay, eControl control) : input(key, delay, control, ControlEmpty)
		{
		}

		input(i8 key, eControl control) : input(key, 10, control)
		{
		}

		i8 m_key{};
		u64 m_delay{};
		bool m_pressed{};
		eControl m_control{};
		eControl m_secondaryControl{};
		i8 m_keyGroup{2};

		operator bool()
		{
			return m_pressed;
		}
	};
}
