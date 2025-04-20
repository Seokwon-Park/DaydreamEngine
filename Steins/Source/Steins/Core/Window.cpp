#include "SteinsPCH.h"
#include "Window.h"

#ifdef STEINS_PLATFORM_WINDOWS
#include "Platform/OSWindows/WindowsWindow.h"
#endif

namespace Steins
{
	Unique<SteinsWindow> SteinsWindow::Create(const WindowProps& _props)
	{


#ifdef STEINS_PLATFORM_WINDOWS
		return MakeUnique<WindowsWindow>(_props);
#elif defined(STEINS_PLATFORM_LINUX)
		return MakeUnique<LinuxWindow>(_props);
#elif  defined(__APPLE__) || defined(__MACH__)
		return MakeUnique<AppleWindow>(_props);
#else
		STS_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}