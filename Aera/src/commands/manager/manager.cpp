#include "manager.h"
#include "memory/pointers.h"

namespace commands
{
	void hotkey::add_hotkey(i32 key)
	{
		m_keys.emplace_back(key);
	}

	bool hotkey::pressed()
	{
		if (m_enabled)
			return false;
		bool pressed{};
		for (auto& k : m_keys)
		{
			if (GetForegroundWindow() == pointers::g_hwnd)
			{
				if (GetAsyncKeyState(k) & 0x1)
				{
					pressed = true;
				}
			}
		}
		return pressed;
	}

	void manager::remove(cc* id)
	{
		if (auto it{m_commands.find(id)}; it != m_commands.end())
		{
			delete it->second;
			m_commands.erase(it);
		}
	}

	void manager::init()
	{
		for (auto& e : m_commands)
		{
			e.second->init();
		}
	}

	void manager::tick()
	{
		for (auto& e : m_commands)
		{
			auto& c{e.second};
			if (c->m_type == eCommandType::ProtectionCommand || c->m_type == eCommandType::SectionProtectionCommand)
				continue;
			if (c->m_looped)
			{
				c->run();
			}
			else if (c->m_hotkey.pressed())
			{
				ONCE_PER_FRAME({ c->run(); });
			}
		}
	}

	void manager::clear()
	{
		for (auto& e : m_commands)
		{
			auto& c{e.second};
			delete c;
		}
	}
}
