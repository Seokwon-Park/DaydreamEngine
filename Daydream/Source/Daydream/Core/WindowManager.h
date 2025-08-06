#pragma once

#include "Window.h"

namespace Daydream
{
	class WindowManager
	{
	public:
		static void Init();
		static DaydreamWindow* GetWindow(String _name);
		static void RegisterWindow(String _name, DaydreamWindow* _window);

	private:
		WindowManager() {};
		static Unique<WindowManager> instance;
		HashMap<std::string, DaydreamWindow*> windows;
	};
}