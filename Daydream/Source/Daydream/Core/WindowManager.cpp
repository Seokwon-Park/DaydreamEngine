#include "DaydreamPCH.h"
#include "WindowManager.h"

namespace Daydream
{
	Unique<WindowManager> WindowManager::instance;

	void WindowManager::Init()
	{
		instance = Unique<WindowManager>(new WindowManager());
	}

	DaydreamWindow* WindowManager::GetWindow(String _name)
	{
		if (instance->windows.find(_name) == instance->windows.end())
		{
			DAYDREAM_CORE_ERROR("Window {} is not registered", _name);
			return nullptr;
		}
		return instance->windows[_name];
	}

	void WindowManager::RegisterWindow(String _name, DaydreamWindow* _window)
	{
		instance->windows.insert({ _name, _window });
	}
}