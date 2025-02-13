#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Steins
{
	class Input
	{
	public:
		static bool GetKeyPress(KeyCode _key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		
	};
}
