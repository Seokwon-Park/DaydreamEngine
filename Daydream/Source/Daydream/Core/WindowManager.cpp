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
}