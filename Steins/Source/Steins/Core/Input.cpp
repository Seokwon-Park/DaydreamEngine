#include "SteinsPCH.h"
#include "Input.h"

#include "Application.h"
#include "Steins/Window.h"
#include "Steins/Event/KeyEvent.h"

namespace Steins
{
	bool Input::GetKeyDown(KeyCode _key)
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		return state == STEINS_PRESS;
	}
	bool Input::GetKeyPress(KeyCode _key)
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		return state == STEINS_REPEAT;
	}
	bool Input::GetKeyUp(KeyCode _key)
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		return state == STEINS_RELEASE;
	}
}
