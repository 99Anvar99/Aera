#pragma once
#include <utility>

#include "pch/pch.h"
#include "gui/data.h"
#include "gui/lerping_float.h"

namespace ui
{
	class tooltip
	{
	public:
		tooltip(
			std::string text, i32 font, float textSize, color textColor, eJustify textJustify,
			bool enabled = true
		) :
			m_text(std::move(text)), m_font(font), m_textSize(textSize), m_textColor(textColor),
			m_textJustify(textJustify),
			m_enabled(enabled)
		{
		}

		void draw() const;

		std::string m_text{};
		i32 m_font{};
		float m_textSize{};
		color m_textColor{};
		eJustify m_textJustify{};
		bool m_enabled{};
	};

	class header
	{
	public:
		header(
			std::string text, float size, color col,
			i32 font, float textSize, color textColor, eJustify textJustify,
			bool enabled = true
		) :
			m_text(std::move(text)), m_size(size), m_color(col),
			m_font(font), m_textSize(textSize), m_textColor(textColor), m_textJustify(textJustify),
			m_enabled(enabled)
		{
		}

		void draw();

		std::string m_text{};
		float m_size{};
		color m_color{};
		i32 m_font{};
		float m_textSize{};
		color m_textColor{};
		eJustify m_textJustify{};
		bool m_enabled{};
	};

	class subtitle
	{
	public:
		subtitle(
			std::string text, float size, float padding, color col,
			i32 font, float textSize, color textColor, eJustify textJustify,
			i32 optionFont, float optionTextSize, color optionTextColor, eJustify optionTextJustify,
			bool enabled = true
		) :
			m_text(std::move(text)), m_size(size), m_padding(padding), m_color(col),
			m_font(font), m_textSize(textSize), m_textColor(textColor), m_textJustify(textJustify),
			m_optionFont(optionFont), m_optionTextSize(optionTextSize), m_optionTextColor(optionTextColor),
			m_optionTextJustify(optionTextJustify),
			m_enabled(enabled)
		{
		}

		void draw();

		std::string m_text{};
		float m_size{};
		float m_padding{};
		color m_color{};
		i32 m_font{};
		float m_textSize{};
		color m_textColor{};
		eJustify m_textJustify{};
		i32 m_optionFont{};
		float m_optionTextSize{};
		color m_optionTextColor{};
		eJustify m_optionTextJustify{};
		bool m_enabled{};
	};

	class options
	{
	public:
		options(
			float size, float padding, color col, color selectedColor,
			i32 font, float textSize, color textColor, color textSelectedColor, eJustify textJustify,
			float scrollSpeed,
			i32 breakFont, float breakTextSize, color breakTextColor, eJustify breakTextJustify,
			sprite arrow, color arrowColor, color arrowSelectedColor, ImVec2 arrowSize,
			sprite toggle, color toggleColor, color toggleSelectedColor, ImVec2 toggleSize
		) :
			m_size(size), m_padding(padding), m_color(col), m_selectedColor(selectedColor),
			m_font(font), m_textSize(textSize), m_textColor(textColor), m_textSelectedColor(textSelectedColor),
			m_textJustify(textJustify),
			m_scrollSpeed(scrollSpeed),
			m_breakFont(breakFont), m_breakTextSize(breakTextSize), m_breakTextColor(breakTextColor),
			m_breakTextJustify(breakTextJustify),
			m_arrow(std::move(arrow)), m_arrowColor(arrowColor), m_arrowSelectedColor(arrowSelectedColor),
			m_arrowSize(arrowSize),
			m_toggle(std::move(toggle)), m_toggleColor(toggleColor), m_toggleSelectedColor(toggleSelectedColor),
			m_toggleSize(toggleSize)
		{
		}

		void draw();

		color rect(bool selected)
		{
			return selected ? m_selectedColor : m_color;
		}

		color text(bool selected)
		{
			return selected ? m_textSelectedColor : m_textColor;
		}

		color arrow(bool selected)
		{
			return selected ? m_arrowSelectedColor : m_arrowColor;
		}

		color toggle(bool selected)
		{
			return selected ? m_toggleSelectedColor : m_toggleColor;
		}

		float m_size{};
		float m_padding{};
		color m_color{};
		color m_selectedColor{};
		i32 m_font{};
		float m_textSize{};
		color m_textColor{};
		color m_textSelectedColor{};
		eJustify m_textJustify{};
		float m_scrollSpeed{};
		i32 m_breakFont{};
		float m_breakTextSize{};
		color m_breakTextColor{};
		eJustify m_breakTextJustify{};
		sprite m_arrow{};
		color m_arrowColor{};
		color m_arrowSelectedColor{};
		ImVec2 m_arrowSize{};
		sprite m_toggle{};
		color m_toggleColor{};
		color m_toggleSelectedColor{};
		ImVec2 m_toggleSize{};
	};

	class footer
	{
	public:
		footer(
			float size, color col,
			sprite sprite, float spriteSize, color spriteColor,
			bool enabled = true
		) :
			m_size(size), m_color(col),
			m_sprite(std::move(sprite)), m_spriteSize(spriteSize), m_spriteColor(spriteColor),
			m_enabled(enabled)
		{
		}

		void draw();

		bool default_sprite()
		{
			if (!m_sprite.m_dictionary.compare("commonmenu"))
			{
				if (!m_sprite.m_texture.compare("shop_arrows_upanddown") || !m_sprite.m_texture.compare("arrowright"))
				{
					return true;
				}
			}
			return false;
		}

		float m_size{};
		color m_color{};
		sprite m_sprite{};
		float m_spriteSize{};
		color m_spriteColor{};
		bool m_enabled{};
	};

	class description
	{
	public:
		description(
			std::string text, float size, float height, float padding, color col,
			i32 font, float textSize, color textColor, eJustify textJustify,
			bool enabled = true
		) :
			m_text(std::move(text)), m_size(size), m_height(height), m_padding(padding), m_color(col),
			m_font(font), m_textSize(textSize), m_textColor(textColor), m_textJustify(textJustify),
			m_enabled(enabled)
		{
		}

		void draw();

		std::string m_text{};
		float m_size{};
		float m_height{};
		float m_padding{};
		color m_color{};
		i32 m_font{};
		float m_textSize{};
		color m_textColor{};
		eJustify m_textJustify{};
		bool m_enabled{};
	};
}
