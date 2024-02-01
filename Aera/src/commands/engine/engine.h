#pragma once
#include "commands/manager/manager.h"
#include "util/player_mgr.h"

namespace commands
{
	class engine
	{
	public:
		util::network::player getPlayerForCommandArgument(const std::string& arg);
		void executeWithCommand(abstractCommand*& Command, const std::string& context);
		bool execute(std::string& string);

		template <typename... T>
		bool primitiveExecute(const std::string fmt, T... args)
		{
			std::string buf{std::vformat(fmt, std::make_format_args(args...))};
			return execute(buf);
		}

		void replaceCommand(abstractCommand* command);
		std::vector<abstractCommand*> findMatches(const std::string& command);
		abstractCommand* getCommand(const std::string& search);
		template <typename t>
		t convertData(const std::string& str);
		void commandFromStream();
		static void commandStreamTick();
		bool m_useDirectMatchResults{};
		bool m_autoComplete{};
		bool m_useFirstResultOnTooManyResults{};
	};

	inline engine g_engine{};
}
