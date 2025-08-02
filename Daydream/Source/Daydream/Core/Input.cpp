#include "DaydreamPCH.h"
#include "Input.h"

#include "Application.h"
#include "Daydream/Core/Window.h"
#include "Daydream/Event/KeyEvent.h"

namespace Daydream
{
	bool Input::GetKeyDown(KeyCode _key)
	{
		DaydreamWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		//_key�� ���� ���� ������
		if (state == Daydream_PRESS && window.GetIsKeyDown(_key) == false)
		{
			//_key�� ���� ������ �����Ѵ�. -> ���� ƽ������ ������ ������ false ����
			window.SetKeyDown(_key);
			return true;
		}
		return false;
	}
	bool Input::GetKeyPressed(KeyCode _key)
	{
		DaydreamWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		return state == Daydream_PRESS || state == Daydream_REPEAT;
	}
	bool Input::GetKeyUp(KeyCode _key)
	{
		DaydreamWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetKeyState(_key);
		if (state == Daydream_RELEASE)
		{
			return true;
		}
		return false;
	}
	bool Input::GetMouseButtonPress(MouseCode _button)
	{
		DaydreamWindow& window = Application::GetInstance().GetMainWindow();
		int state = window.GetMouseState(_button);
		return state == Daydream_PRESS;
	}
	Pair<Float32, Float32> Input::GetMousePosition()
	{
		DaydreamWindow& window = Application::GetInstance().GetMainWindow();
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
