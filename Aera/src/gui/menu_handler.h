#pragma once
#include "types/submenu.h"

namespace ui
{
	template <typename T>
	class menuInstance : public submenu
	{
	public:
		menuInstance(const std::string& name, const fnptr<void(submenu&)> callback) : submenu(name, callback)
		{
		}

		static menuInstance* get(std::string name, fnptr<void(submenu&)> callback)
		{
			static std::shared_ptr<menuInstance> instance{};
			if (!instance)
			{
				instance = std::make_shared<menuInstance>(name, callback);
				instance->once();
			}
			return instance.get();
		}
	};
}
