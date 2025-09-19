#pragma once

#include "Window.h"
#include "RegistryBase.h"

namespace Daydream
{
	class WindowManager : public RegistryBase<String, DaydreamWindow*>
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