#include "DaydreamPCH.h"
#include "WindowManager.h"

namespace Daydream
{
	void WindowManager::Init()
	{
		instance = new WindowManager();
	}

	void WindowManager::Shutdown()
	{
		delete instance;
	}

	void WindowManager::RegisterWindow(String _name, DaydreamWindow* _window)
	{
		instance->windows.insert({ _name, _window });
	}
	DaydreamWindow* WindowManager::GetWindow(String _name)
	{
		auto itr = instance->windows.find(_name);
		if (itr == instance->windows.end())
		{
			DAYDREAM_CORE_WARN("{} is not valid Window name!", _name);
			return nullptr;
		}
		return itr->second;
	}
}