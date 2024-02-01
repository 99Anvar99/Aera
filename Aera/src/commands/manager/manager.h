#pragma once
#include "commands/manager/types.h"
#include "util/util.h"

namespace commands
{
	inline bool g_setConfig{};

	class manager
	{
	public:
		template <typename t>
		void add(t command)
		{
			t* cmd{new t(command)};
			m_commands.insert({cmd->id(), std::move(cmd)});
		}

		void remove(cc* id);
		void init();
		void tick();
		void clear();

		template <typename t>
		t* getCommand(cc* id)
		{
			for (auto& e : m_commands)
			{
				auto& c{e.second};
				if (c->m_id == id)
				{
					return static_cast<t*>(c);
				}
			}
			return nullptr;
		}

		std::map<cc*, abstractCommand*>& getCommands()
		{
			return m_commands;
		}

	private:
		std::map<cc*, abstractCommand*> m_commands{};
	};

	inline manager g_manager{};
}

inline commands::abstractCommand* operator ""_ABC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::abstractCommand>(str);
}

inline commands::toggleCommand* operator ""_TC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::toggleCommand>(str);
}

inline commands::intCommand* operator ""_IC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::intCommand>(str);
}

inline commands::toggleIntCommand* operator ""_TIC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::toggleIntCommand>(str);
}

inline commands::floatCommand* operator ""_FC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::floatCommand>(str);
}

inline commands::toggleFloatCommand* operator ""_TFC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::toggleFloatCommand>(str);
}

inline commands::actionCommand* operator ""_AC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::actionCommand>(str);
}

inline commands::protectionCommand* operator ""_PC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::protectionCommand>(str);
}

inline commands::sectionProtectionCommand* operator ""_SPC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::sectionProtectionCommand>(str);
}

inline commands::stringCommand* operator ""_SC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::stringCommand>(str);
}

inline commands::hashCommand* operator ""_HC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::hashCommand>(str);
}

inline commands::colorCommand* operator ""_CC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::colorCommand>(str);
}

inline commands::variadicCommand* operator ""_VC(cc* str, u64)
{
	return commands::g_manager.getCommand<commands::variadicCommand>(str);
}
