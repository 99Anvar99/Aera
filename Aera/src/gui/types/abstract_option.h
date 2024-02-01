#pragma once
#include "pch/pch.h"

namespace ui
{
	enum class eOptionTypes : u32
	{
		Abstract,
		Break,
		Regular,
		Submenu,
		Player,
		Toggle,
		Number,
		Vector
	};

	enum class eActionType : u32
	{
		Enter,
		Up,
		Down,
		Left,
		Right
	};

	class abstractOption
	{
	public:
		abstractOption(std::string& name, std::string& description) : m_name(name), m_description(description)
		{
		}

		virtual ~abstractOption() = default;

		virtual void draw(bool selected)
		{
		}

		virtual void action(eActionType type)
		{
		}

		virtual u32 type() { return static_cast<u32>(eOptionTypes::Abstract); }

		std::string m_name{};
		std::string m_description{};
	};
}
