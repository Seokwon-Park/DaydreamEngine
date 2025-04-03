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
		if (state == STEINS_PRESS && window.GetIsKeyDown(_key) == false)
		{
			window.SetKeyDown(_key);
			return true;
		}
		return false;
	}
	bool Input::GetKeyPress(KeyCode _key)
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		return state == STEINS_PRESS || state == STEINS_REPEAT;
	}
	bool Input::GetKeyUp(KeyCode _key)
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		if (state == STEINS_RELEASE)
		{
			return true;
		}
		return false;
	}
	bool Input::GetMouseButtonPress(MouseCode _button)
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetMouseState(_button);
		return state == STEINS_PRESS;
	}
	Pair<Float32, Float32> Input::GetMousePosition()
	{
		SteinsWindow& window = Application::GetInstance().GetMainWindow();
		Pair<Float32, Float32> mousePos = window.GetMousePos();
		return mousePos;
	}
	Float32 Input::GetMouseX()
	{
		return GetMousePosition().first;
	}
	Float32 Input::GetMouseY()
	{
		return GetMousePosition().second;
	}
}
