#include "SteinsPCH.h"
#include "Window.h"

#ifdef STEINS_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Steins
{
	Unique<SteinsWindow> SteinsWindow::Create(const WindowProps& _props)
	{
#ifdef STEINS_PLATFORM_WINDOWS
		return MakeUnique<WindowsWindow>(_props);
#else
		STS_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

	void SteinsWindow::InputKey(int _key, int _action)
	{
		bool repeated = false;

		if (_key >= 0 && _key <= STEINS_KEY_LAST)
		{
			if (_action == STEINS_KEY_RELEASE && keyStates[_key] == STEINS_KEY_RELEASE)
				return;

			if (_action == STEINS_KEY_PRESS && keyStates[_key] == STEINS_KEY_PRESS)
				repeated = true;

			//if (_action == STEINS_KEY_RELEASE && window->stickyKeys)
			//	window->keys[key] = #STICK;
			
			keyStates[_key] = (char)_action;

			if (repeated)
				_action = STEINS_KEY_REPEAT;

			if (callbacks.inputKeyFn != nullptr)
			{
				callbacks.inputKeyFn(this, _key, _action);
			}
		}
	}


}