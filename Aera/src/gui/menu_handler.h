#pragma once
#include "types/submenu.h"

namespace ui
{
	template <typename t>
	class menuInstance : public submenu
	{
	public:
		menuInstance(std::string name, fnptr<void(submenu&)> callback) : submenu(name, callback)
		{
		}

	public:
		static menuInstance<t>* get(std::string name, fnptr<void(submenu&)> callback)
		{
			static std::shared_ptr<menuInstance<t>> instance{};
			if (!instance)
			{
				instance = std::make_shared<menuInstance<t>>(name, callback);
				instance->once();
			}
			return instance.get();
		}
	};
}
