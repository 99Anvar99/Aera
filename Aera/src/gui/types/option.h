#pragma once
#include "base_option.h"

namespace ui
{
	class option : public baseOption
	{
	public:
		option(std::string name, std::string description, std::function<void()> action = {}) :
			baseOption(name, description, action)
		{
		}

		option(std::string name, std::function<void()> action = {}) :
			option(name, {}, action)
		{
		}

		option(commands::actionCommand* cmd) :
			option(cmd->m_name, cmd->m_description, [cmd] { cmd->run(); })
		{
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Regular);
		}
	};
}
