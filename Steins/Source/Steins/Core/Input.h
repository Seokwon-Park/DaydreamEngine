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
		static Pair<Float32, Float32> GetMousePosition();
		static Float32 GetMouseX();
		static Float32 GetMouseY();
			
	};
}
