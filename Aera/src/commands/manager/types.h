#pragma once
#include "abstract.h"
#include "gui/data.h"

enum class eProtectionState : i8
{
	Disabled,
	Notify,
	Block,
	BlockAndNotify
};

inline cc* g_protectionStates[4]{
	"Disabled",
	"Notify",
	"Block",
	"Block And Notify"
};

namespace commands
{
	inline eProtectionState getProtectionStateFromString(const std::string& string)
	{
		std::string str{string_to_lower(string)};
		if (!str.compare("ban"))
		{
			return eProtectionState::BlockAndNotify;
		}
		if (!str.compare("bandn"))
		{
			return eProtectionState::BlockAndNotify;
		}
		for (i8 i{}; i != COUNT(g_protectionStates); ++i)
		{
			if (!str.compare(string_to_lower(g_protectionStates[i])) || string_to_lower(g_protectionStates[i]).find(str)
				!= std::string::npos)
			{
				return static_cast<eProtectionState>(i);
			}
		}
		return eProtectionState::Disabled;
	}

	class toggleCommand : public abstractCommand
	{
	public:
		toggleCommand(const std::string& id, const std::string& name, const std::string& description, hotkey hotkey,
		              fnptr<void(toggleCommand*)> callback) :
			abstractCommand(id, name, description, hotkey, eCommandType::ToggleCommand, true),
			m_callback(callback)
		{
		}

		toggleCommand(const std::string& id, const std::string& name, const std::string& description,
		              fnptr<void(toggleCommand*)> callback) :
			toggleCommand(id, name, description, {}, callback)
		{
		}

		toggleCommand(const std::string& id, const std::string& name, fnptr<void(toggleCommand*)> callback) :
			toggleCommand(id, name, {}, callback)
		{
		}

		~toggleCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_value);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_hotkey.pressed())
			{
				m_value.m_value.toggle ^= true;
			}
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		typedValue m_value{};

	private:
		fnptr<void(toggleCommand*)> m_callback{};
	};

	class intCommand : public abstractCommand
	{
	public:
		intCommand(const std::string& id, const std::string& name, const std::string& description, hotkey hotkey,
		           fnptr<void(intCommand*)> callback, bool looped = false) :
			abstractCommand(id, name, description, hotkey, eCommandType::IntCommand, looped),
			m_callback(callback)
		{
		}

		intCommand(const std::string& id, const std::string& name, const std::string& description,
		           fnptr<void(intCommand*)> callback, bool looped = false) :
			intCommand(id, name, description, {}, callback, looped)
		{
		}

		intCommand(const std::string& id, const std::string& name, fnptr<void(intCommand*)> callback,
		           bool looped = false) :
			intCommand(id, name, {}, callback, looped)
		{
		}

		~intCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_value);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		typedValue m_value{};

	private:
		fnptr<void(intCommand*)> m_callback{};
	};

	class toggleIntCommand : public abstractCommand
	{
	public:
		toggleIntCommand(const std::string& id, const std::string& name, const std::string& description, hotkey hotkey,
		                 fnptr<void(toggleIntCommand*)> callback) :
			abstractCommand(id, name, description, hotkey, eCommandType::ToggleIntCommand, true),
			m_callback(callback)
		{
		}

		toggleIntCommand(const std::string& id, const std::string& name, const std::string& description,
		                 fnptr<void(toggleIntCommand*)> callback) :
			toggleIntCommand(id, name, description, {}, callback)
		{
		}

		toggleIntCommand(const std::string& id, const std::string& name, fnptr<void(toggleIntCommand*)> callback) :
			toggleIntCommand(id, name, {}, callback)
		{
		}

		~toggleIntCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_toggleValue);
			push_value(m_numberValue);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_hotkey.pressed())
			{
				m_toggleValue.m_value.toggle ^= true;
			}
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		typedValue m_toggleValue{};
		typedValue m_numberValue{};

	private:
		fnptr<void(toggleIntCommand*)> m_callback{};
	};

	class floatCommand : public abstractCommand
	{
	public:
		floatCommand(const std::string& id, const std::string& name, const std::string& description, hotkey hotkey,
		             fnptr<void(floatCommand*)> callback, bool looped = false) :
			abstractCommand(id, name, description, hotkey, eCommandType::FloatCommand, looped),
			m_callback(callback)
		{
		}

		floatCommand(const std::string& id, const std::string& name, const std::string& description,
		             fnptr<void(floatCommand*)> callback, bool looped = false) :
			floatCommand(id, name, description, {}, callback, looped)
		{
		}

		floatCommand(const std::string& id, const std::string& name, fnptr<void(floatCommand*)> callback,
		             bool looped = false) :
			floatCommand(id, name, {}, callback, looped)
		{
		}

		~floatCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_value);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		typedValue m_value{};

	private:
		fnptr<void(floatCommand*)> m_callback{};
	};

	class toggleFloatCommand : public abstractCommand
	{
	public:
		toggleFloatCommand(const std::string& id, const std::string& name, const std::string& description,
		                   hotkey hotkey, fnptr<void(toggleFloatCommand*)> callback) :
			abstractCommand(id, name, description, hotkey, eCommandType::ToggleFloatCommand, true),
			m_callback(callback)
		{
		}

		toggleFloatCommand(const std::string& id, const std::string& name, const std::string& description,
		                   fnptr<void(toggleFloatCommand*)> callback) :
			toggleFloatCommand(id, name, description, {}, callback)
		{
		}

		toggleFloatCommand(const std::string& id, const std::string& name, fnptr<void(toggleFloatCommand*)> callback) :
			toggleFloatCommand(id, name, {}, callback)
		{
		}

		~toggleFloatCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_toggleValue);
			push_value(m_numberValue);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_hotkey.pressed())
			{
				m_toggleValue.m_value.toggle ^= true;
			}
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		typedValue m_toggleValue{};
		typedValue m_numberValue{};

	private:
		fnptr<void(toggleFloatCommand*)> m_callback{};
	};

	class actionCommand : public abstractCommand
	{
	public:
		actionCommand(const std::string& id, const std::string& name, const std::string& description, hotkey hotkey,
		              fnptr<void(actionCommand*)> callback) :
			abstractCommand(id, name, description, hotkey, eCommandType::ActionCommand, false),
			m_callback(callback)
		{
		}

		actionCommand(const std::string& id, const std::string& name, const std::string& description,
		              fnptr<void(actionCommand*)> callback) :
			actionCommand(id, name, description, {}, callback)
		{
		}

		actionCommand(const std::string& id, const std::string& name, fnptr<void(actionCommand*)> callback) :
			actionCommand(id, name, {}, callback)
		{
		}

		~actionCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

	private:
		fnptr<void(actionCommand*)> m_callback{};
	};

	class protectionCommand : public abstractCommand
	{
	public:
		protectionCommand(const std::string& id, const std::string& name, const std::string& description) :
			abstractCommand(id, name, description, {}, eCommandType::ProtectionCommand, false)
		{
		}

		protectionCommand(const std::string& id, const std::string& name) :
			protectionCommand(id, name, {})
		{
		}

		~protectionCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_value);
			abstractCommand::init();
		}

		void run() override
		{
			abstractCommand::run();
		}

		void update(cc* n)
		{
			m_value.m_value.string = (char*)n;
			setState();
		}

		void setFromSection(eProtectionState state)
		{
			m_state = state;
		}

		void setState()
		{
			m_state = getProtectionStateFromString(m_value.m_value.string);
		}

		eProtectionState state()
		{
			return m_state;
		}

		eProtectionState m_state{};

	private:
		typedValue m_value{};
	};

	class sectionProtectionCommand : public abstractCommand
	{
	public:
		sectionProtectionCommand(const std::string& id, const std::string& name, const std::string& description,
		                         fnptr<void(sectionProtectionCommand*)> callback) :
			abstractCommand(id, name, description, {}, eCommandType::SectionProtectionCommand, false),
			m_callback(callback)
		{
		}

		sectionProtectionCommand(const std::string& id, const std::string& name,
		                         fnptr<void(sectionProtectionCommand*)> callback) :
			sectionProtectionCommand(id, name, {}, callback)
		{
		}

		~sectionProtectionCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_value);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		void update(cc* n)
		{
			m_value.m_value.string = (char*)n;
			setState();
		}

		void setState()
		{
			m_state = getProtectionStateFromString(m_value.m_value.string);
		}

		eProtectionState state()
		{
			return m_state;
		}

		eProtectionState m_state{};

	private:
		typedValue m_value{};
		fnptr<void(sectionProtectionCommand*)> m_callback{};
	};

	class stringCommand : public abstractCommand
	{
	public:
		stringCommand(const std::string& id, const std::string& name, const std::string& description,
		              fnptr<void(stringCommand*)> callback) :
			abstractCommand(id, name, description, {}, eCommandType::StringCommand, false), m_callback(callback)
		{
		}

		stringCommand(const std::string& id, const std::string& name, fnptr<void(stringCommand*)> callback) :
			stringCommand(id, name, {}, callback)
		{
		}

		void init() override
		{
			push_value(m_stringValue);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		void set_string(const std::string& str)
		{
			m_storedString = str;
			m_stringValue.m_value.string = (char*)m_storedString.c_str();
		}

		std::string get_string()
		{
			return m_stringValue.m_value.string;
		}

		std::string get_context()
		{
			std::vector<std::string> words{get_matches(m_context, R"_(\S+)_")};
			return m_context.substr(m_context.find(words.at(0)) + words[0].size());
		}

	private:
		std::string m_storedString{};
		typedValue m_stringValue{};
		fnptr<void(stringCommand*)> m_callback{};
	};

	class hashCommand : public abstractCommand
	{
	public:
		hashCommand(const std::string& id, const std::string& name, const std::string& description,
		            fnptr<void(hashCommand*)> callback) :
			abstractCommand(id, name, description, {}, eCommandType::HashCommand, false), m_callback(callback)
		{
		}

		hashCommand(const std::string& id, const std::string& name, fnptr<void(hashCommand*)> callback) :
			hashCommand(id, name, {}, callback)
		{
		}

		void init() override
		{
			push_value(m_stringValue);
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

		void set_key(std::string str)
		{
			m_storedKey = str;
			m_stringValue.m_value.string = (char*)m_storedKey.c_str();
		}

		std::string get_key()
		{
			return m_stringValue.m_value.string;
		}

		u32 get_hash()
		{
			std::string key{get_key()};
			u32 hash{};
			if (is_number(key))
			{
				hash = stoi(key);
			}
			else if (contains_an_number(key) && (key.size() == 8 || key.size() == 6))
			{
				hash = stoul(key);
			}
			else
			{
				hash = rage::joaat(key);
			}
			return hash;
		}

	private:
		std::string m_storedKey{};
		typedValue m_stringValue{};
		fnptr<void(hashCommand*)> m_callback{};
	};

	class colorCommand : public abstractCommand
	{
	public:
		colorCommand(const std::string& id, const std::string& name, const std::string& description) :
			abstractCommand(id, name, description, {}, eCommandType::ColorCommand, false)
		{
		}

		colorCommand(const std::string& id, const std::string& name) :
			colorCommand(id, name, {})
		{
		}

		~colorCommand() override
		{
			abstractCommand::~abstractCommand();
		}

		void init() override
		{
			push_value(m_rValue);
			push_value(m_gValue);
			push_value(m_bValue);
			push_value(m_aValue);
			abstractCommand::init();
		}

		void run() override
		{
			abstractCommand::run();
		}

		ui::color get_color()
		{
			return {(*m_rValue).u8, (*m_gValue).u8, (*m_bValue).u8, (*m_aValue).u8};
		}

	private:
		typedValue m_rValue{};
		typedValue m_gValue{};
		typedValue m_bValue{};
		typedValue m_aValue{};
	};

	class variadicCommand : public abstractCommand
	{
	public:
		variadicCommand(const std::string& id, const std::string& name, const std::string& description,
		                std::vector<typedValue> values, fnptr<void(variadicCommand*)> callback, bool looped) :
			abstractCommand(id, name, description, {}, eCommandType::VariadicCommand, looped),
			m_callback(callback), m_values(values), m_loop(looped)
		{
		}

		variadicCommand(const std::string& id, const std::string& name, std::vector<typedValue> values,
		                fnptr<void(variadicCommand*)> callback, bool looped) :
			variadicCommand(id, name, {}, values, callback, looped)
		{
		}

		void init() override
		{
			for (auto& v : m_values)
			{
				push_value(v);
			}
			abstractCommand::init();
		}

		void run() override
		{
			if (m_callback)
			{
				m_callback(this);
			}
			abstractCommand::run();
		}

	private:
		fnptr<void(variadicCommand*)> m_callback{};
		std::vector<typedValue> m_values{};
		bool m_loop{};
	};
}
