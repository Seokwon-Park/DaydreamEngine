#include "DaydreamPCH.h"
#include "Window.h"

#ifdef DAYDREAM_PLATFORM_WINDOWS
#include "Platform/OSWindows/WindowsWindow.h"
#endif

namespace Daydream
{
	Unique<DaydreamWindow> DaydreamWindow::Create(const WindowDesc& _desc)
	{
#ifdef DAYDREAM_PLATFORM_WINDOWS
		return MakeUnique<WindowsWindow>(_desc);
#elif defined(DAYDREAM_PLATFORM_LINUX)
		return MakeUnique<LinuxWindow>(_desc);
#elif  defined(__APPLE__) || defined(__MACH__)
		return MakeUnique<AppleWindow>(_desc);
#else
		DAYDREAM_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}