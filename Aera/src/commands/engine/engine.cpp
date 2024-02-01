#include "engine.h"
#include "util/util.h"
#include "fiber/pool.h"

namespace commands
{
	util::network::player engine::getPlayerForCommandArgument(const std::string& arg)
	{
		util::network::player p{};
		if (is_number(arg))
		{
			u32 index{stoul(arg)};
			if (index > util::network::g_manager.m_playerCount)
			{
				LOG(Commands, "The index '{}' provided is out of range! Please provide a name or valid index.", index);
				return {};
			}
			p = util::network::g_manager[index];
		}
		else
		{
			std::string name{arg};
			for (auto& entry : util::network::g_manager)
			{
				if (auto& player{entry.second}; player.valid())
				{
					if (player.m_name == name)
					{
						p = player;
						break;
					}
					if (player.m_name.find(name) != -1)
					{
						LOG(Commands, "The name '{}' isn't unique enough! Please try again", name);
						break;
					}
				}
			}
		}
		return p;
	}

	void engine::executeWithCommand(abstractCommand*& command, const std::string& context)
	{
		auto arguments{get_matches(context, R"_(\S+)_")};
		size_t trueArgCount{arguments.size() - 1};
		switch (command->m_type)
		{
		case eCommandType::ToggleCommand:
		case eCommandType::VariadicCommand:
		case eCommandType::StringCommand:
		case eCommandType::HashCommand:
			{
			}
			break;
		case eCommandType::ActionCommand:
			{
				if (trueArgCount > 0)
				{
					LOG(Commands, "You provided {} arguments for a command that requires no arguments.", trueArgCount);
					return;
				}
			}
			break;
		case eCommandType::ToggleFloatCommand:
		case eCommandType::ToggleIntCommand:
			{
				if (trueArgCount > 2)
				{
					LOG(Commands, "You provided {} arguments for a command that requires 2 arguments.",
					    arguments.size());
					return;
				}
			}
			break;
		case eCommandType::ColorCommand:
			{
				if (trueArgCount >= 3)
				{
					LOG(Commands, "You provided {} arguments for a command that requiresat least 3 arguments.",
					    arguments.size());
					return;
				}
			}
			break;
		default:
			{
				if (trueArgCount > 1)
				{
					LOG(Commands, "You provided {} arguments for a command that requires one argument.", trueArgCount);
					return;
				}
			}
			break;
		}
		//Handle argument parsing
		switch (command->m_type)
		{
		case eCommandType::ToggleCommand:
			{
				command->get(0).toggle = convertData<bool>(arguments[1]);
			}
			break;
		case eCommandType::IntCommand:
			{
				command->get(0).i32 = convertData<i32>(arguments[1]);
			}
			break;
		case eCommandType::FloatCommand:
			{
				command->get(0).floating_point = convertData<float>(arguments[1]);
			}
			break;
		case eCommandType::ToggleIntCommand:
			{
				command->get(0).toggle = convertData<bool>(arguments[1]);
				command->get(1).i32 = convertData<i32>(arguments[2]);
			}
			break;
		case eCommandType::ToggleFloatCommand:
			{
				command->get(0).toggle = convertData<bool>(arguments[1]);
				command->get(1).floating_point = convertData<float>(arguments[2]);
			}
			break;
		case eCommandType::ActionCommand:
			{
				//Handled below, no arguments required.
			}
			break;
		case eCommandType::ProtectionCommand:
			{
				auto cmd{static_cast<protectionCommand*>(command)};
				cmd->get(0).string = (char*)arguments[1].c_str();
				cmd->update(cmd->get(0).string);
			}
			break;
		case eCommandType::SectionProtectionCommand:
			{
				auto cmd{static_cast<sectionProtectionCommand*>(command)};
				cmd->get(0).string = (char*)arguments[1].c_str();
				cmd->update(cmd->get(0).string);
			}
			break;
		case eCommandType::StringCommand:
			{
				auto cmd{static_cast<stringCommand*>(command)};
				cmd->m_buffer.clear();
				cmd->m_context = context;
				size_t index{context.find(arguments[1])};
				cmd->m_buffer.push_back(context.substr(index));
				cmd->set_string(cmd->m_buffer[0]);
			}
			break;
		case eCommandType::HashCommand:
			{
				auto cmd{static_cast<hashCommand*>(command)};
				cmd->m_buffer.clear();
				cmd->m_context = context;
				size_t index{context.find(arguments[1])};
				cmd->m_buffer.push_back(context.substr(index));
				cmd->set_key(arguments[1]);
			}
			break;
		case eCommandType::ColorCommand:
			{
				auto cmd{static_cast<colorCommand*>(command)};
				//This will make 255, into 255
				//A bad side effect is 2,5,5, 1,,9,,,2, 2,,,,5,,,,5,,, will become valid, but fuck it. I don't care enough
				cmd->get(0).u8 = convertData<u8>(trim_string(arguments[1], ','));
				cmd->get(1).u8 = convertData<u8>(trim_string(arguments[2], ','));
				cmd->get(2).u8 = convertData<u8>(trim_string(arguments[3], ','));
				if (arguments.size() == 4)
				{
					cmd->get(3).u8 = convertData<u8>(arguments[4]);
				}
			}
			break;
		case eCommandType::VariadicCommand:
			{
				if (command->has_value())
				{
					if (command->get_value(0)->m_type != eValueType::String)
					{
						if (command->value_count() != trueArgCount)
						{
							LOG(Commands, "You provided {} arguments for a command that requires {} arguments.",
							    trueArgCount, command->value_count());
							return;
						}
						for (size_t i{0}; i != trueArgCount; ++i)
						{
							typedValue& value{*command->get_value(i)};
							std::string arg{arguments[i + 1]};
							switch (value.m_type)
							{
							case eValueType::String:
								{
									strncpy((*value).string, arg.c_str(), arg.size());
								}
								break;
							case eValueType::Boolean:
								{
									(*value).toggle = convertData<bool>(arg);
								}
								break;
							case eValueType::FloatingPoint:
								{
									(*value).floating_point = convertData<float>(trim_string(arg, ','));
								}
								break;
							case eValueType::Int8:
								{
									(*value).i8 = convertData<i8>(arg);
								}
								break;
							case eValueType::UInt8:
								{
									(*value).u8 = convertData<u8>(arg);
								}
								break;
							case eValueType::Int16:
								{
									(*value).i16 = convertData<i16>(arg);
								}
								break;
							case eValueType::UInt16:
								{
									(*value).u16 = convertData<u16>(arg);
								}
								break;
							case eValueType::Int32:
								{
									(*value).i32 = convertData<i32>(arg);
								}
								break;
							case eValueType::UInt32:
								{
									(*value).u32 = convertData<u32>(arg);
								}
								break;
							case eValueType::Int64:
								{
									(*value).i64 = convertData<i64>(arg);
								}
								break;
							case eValueType::UInt64:
								{
									(*value).u64 = convertData<u64>(arg);
								}
								break;
							case eValueType::GamePlayer:
								{
									(*value).game_player = getPlayerForCommandArgument(arg).m_netGamePlayer;
								}
								break;
							case eValueType::NetPlayer:
								{
									(*value).net_player = getPlayerForCommandArgument(arg).m_netGamePlayer;
								}
								break;
							case eValueType::GamerInfo:
								{
									(*value).gamer_info = getPlayerForCommandArgument(arg).m_gamerInfo;
								}
								break;
							}
						}
					}
					else
					{
						command->m_buffer.clear();
						command->m_context = context;
						if (command->value_count() == 1)
						{
							size_t index{context.find(arguments[1])};
							command->m_buffer.push_back(context.substr(index));
							command->get(0).string = (char*)command->m_buffer[0].c_str();
						}
						else
						{
							command->m_buffer.push_back("Reversed");
							//Args start at index 1, this is so we don't fuck things up when reading args
							for (size_t i{1}; i != command->value_count(); ++i)
							{
								command->m_buffer.push_back(arguments[i]);
								strncpy(command->get(i).string, command->m_buffer[i].c_str(),
								        command->m_buffer[i].size());
							}
						}
					}
				}
			}
			break;
		default:
			{
			}
			break;
		}
		if (!command->m_looped)
			command->run();
	}

	bool engine::execute(std::string& string)
	{
		if (!string.size())
		{
			LOG(Commands, "Empty command string!");
			return false;
		}
		auto words{get_matches(string, R"_(\S+)_")};
		if (words.empty())
		{
			LOG(Commands, "No command!");
			return false;
		}
		if (is_number(words[0]))
		{
			LOG(Commands, "Provide a command!");
			return false;
		}
		abstractCommand* command{getCommand(words[0])};
		if (!command)
			return false;
		if (words.size() == 1)
		{
			switch (command->m_type)
			{
			case eCommandType::ToggleIntCommand:
			case eCommandType::ToggleFloatCommand:
			case eCommandType::ToggleCommand:
				{
					command->get(0).toggle ^= true;
					replaceCommand(command);
					return true;
				}
				break;
			case eCommandType::ActionCommand:
				{
					command->run();
					return true;
				}
				break;
			}
			LOG(Commands, "You provided no arguments for a command that requires arguments!");
			return false;
		}
		if (words.size() > 1)
		{
			if (words.size() == 2)
			{
				switch (command->m_type)
				{
				case eCommandType::ToggleIntCommand:
					{
						if (contains_an_number(words[1]) || is_number(words[1]))
							command->get(1).i32 = convertData<i32>(words[1]);
						else
							command->get(0).toggle = convertData<bool>(words[1]);
						replaceCommand(command);
						return true;
					}
					break;
				case eCommandType::ToggleFloatCommand:
					{
						if (contains_an_number(words[1]) || is_number(words[1]))
							command->get(1).floating_point = convertData<float>(words[1]);
						else
							command->get(0).toggle = convertData<bool>(words[1]);
						replaceCommand(command);
						return true;
					}
					break;
				}
			}
			executeWithCommand(command, string.substr(string.find(words[0])));
			string = m_autoComplete ? command->m_id : string;
			replaceCommand(command);
			return true;
		}
		return false;
	}

	void engine::replaceCommand(abstractCommand* command)
	{
		for (auto& f : g_manager.getCommands())
		{
			if (f.second->m_id == command->m_id)
			{
				f.second = command;
				break;
			}
		}
	}

	std::vector<abstractCommand*> engine::findMatches(const std::string& command)
	{
		std::string lower{string_to_lower(command)};
		std::vector<abstractCommand*> matches{};
		for (auto& f : g_manager.getCommands())
		{
			if (string_to_lower(f.second->m_id).find(lower) != std::string::npos)
			{
				matches.push_back(f.second);
			}
		}
		return matches;
	}

	abstractCommand* engine::getCommand(const std::string& search)
	{
		auto matches{findMatches(search)};
		if (matches.empty())
		{
			LOG(Commands, "'{}' isn't a valid command.", search);
			return nullptr;
		}
		if (m_useDirectMatchResults)
		{
			std::string lower{string_to_lower(search)};
			for (auto& m : matches)
			{
				if (string_to_lower(m->m_id) == lower)
				{
					return m;
				}
			}
		}
		if (matches.size() > 1)
		{
			if (m_useFirstResultOnTooManyResults)
			{
				LOG(Commands, "'{}' isn't unique enough. Using {} instead.", search, matches[0]->m_id);
				return matches[0];
			}
			LOG(Commands, "'{}' isn't unique enough. Maybe you meant {}?", search, matches[0]->m_id);
		}
		return matches[0];
		return nullptr;
	}

	template <typename t>
	t engine::convertData(const std::string& str)
	{
		if (is_number(str))
		{
			return static_cast<t>(stod(str));
		}
		if (contains_an_number(str))
		{
			if (str.find('.') == std::string::npos)
			{
				if (str.find("0x") != std::string::npos || str.find("0X") != std::string::npos)
				{
					return static_cast<t>(stoull(str));
				}
			}
			else
			{
				return static_cast<t>(stod(str));
			}
		}
		else
		{
			if (str == "on")
			{
				return static_cast<t>(true);
			}
			if (str == "off")
			{
				return static_cast<t>(false);
			}
			if (str == "true")
			{
				return static_cast<t>(true);
			}
			if (str == "false")
			{
				return static_cast<t>(false);
			}
			if (str == "t")
			{
				return static_cast<t>(true);
			}
			if (str == "f")
			{
				return static_cast<t>(false);
			}
		}
		return static_cast<t>(NULL);
	}

	void engine::commandFromStream()
	{
		auto file{util::files::input("Command.txt")};
		std::string stream{util::files::read(file)};
		file.close();
		if (!stream.empty())
		{
			{
				auto file{util::files::output("Command.txt")};
				util::files::destory(file);
			}
			execute(stream);
		}
	}

	void engine::commandStreamTick()
	{
		while (true)
		{
			//This is typically used for serialising commandss from other processes
			g_engine.commandFromStream();
			fiber::current()->sleep(2s);
		}
	}
}
