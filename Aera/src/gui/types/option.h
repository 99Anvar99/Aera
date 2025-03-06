#pragma once
#include "base_option.h"

namespace ui
{
	class option : public baseOption
	{
	public:
		option(const std::string& name, const std::string& description, const std::function<void()>& action = {}) :
			baseOption(name, description, action)
		{
		}

		option(const std::string& name, const std::function<void()>& action = {}) :
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
