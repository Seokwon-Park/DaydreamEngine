#pragma once

#include "Window.h"

namespace Daydream
{
	class WindowManager 
	{
	public:
		static void Init();
		static void Shutdown();
		static void RegisterWindow(String _name, DaydreamWindow* _window);

	private:
		WindowManager() {};
		inline static WindowManager* instance = nullptr;
		HashMap<std::string, DaydreamWindow*> windows;
	};
}