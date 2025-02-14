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
#elif defined(STEINS_PLATFORM_LINUX)
		return MakeUnique<LinuxWindow>(_props);
#else
		STS_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

	void SteinsWindow::WindowInputKey(int _key, int _action)
	{
		bool repeated = false;

		if (_key >= 0 && _key <= STEINS_KEY_LAST)
		{
			if (_action == STEINS_RELEASE && keyStates[_key] == STEINS_RELEASE)
			{
				return;
			}

			if (_action == STEINS_PRESS && keyStates[_key] == STEINS_PRESS)
			{
				repeated = true;
			}

			//if (_action == STEINS_KEY_RELEASE && window->stickyKeys)
			//	window->keys[key] = #STICK;
			
			keyStates[_key] = (char)_action;

			if (repeated)
				_action = STEINS_REPEAT;

			if (callbacks.inputKeyFn != nullptr)
			{
				callbacks.inputKeyFn(this, _key, _action);
			}
		}
	}

	void SteinsWindow::WindowMouseClick(int _button, int _action)
	{
		bool repeated = false;

		if (_button < 0 || _button > STEINS_MOUSE_BUTTON_LAST)
			return;

		mouseStates[_button] = (char)_action;

		if (callbacks.mouseButtonFn!=nullptr)
		{
			callbacks.mouseButtonFn(this, _button, _action);
		}
	}

	void SteinsWindow::WindowMouseScroll(double _xOffset, double _yOffset)
	{
		if (callbacks.mouseScrollFn != nullptr)
		{
			callbacks.mouseScrollFn(this, _xOffset, _yOffset);
		}
	}

	void SteinsWindow::WindowCursorPos(double _xPos, double _yPos)
	{
		if (callbacks.cursorPosFn != nullptr)
		{
			callbacks.cursorPosFn(this, _xPos, _yPos);
		}
	}


	SteinsWindow::SteinsWindow()
	{
		keyStates.resize(STEINS_KEY_LAST, 0);
		mouseStates.resize(STEINS_MOUSE_BUTTON_LAST+1, 0);

	}

}