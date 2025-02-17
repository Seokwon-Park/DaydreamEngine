#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Steins
{
	class Input
	{
	public:
		static bool GetKeyDown(KeyCode _key);
		static bool GetKeyPress(KeyCode _key);
		static bool GetKeyUp(KeyCode _key);

		static bool GetMouseButtonPress(MouseCode button);
		static Pair<float32, float32> GetMousePosition();
		static float32 GetMouseX();
		static float32 GetMouseY();
			
	};
}
