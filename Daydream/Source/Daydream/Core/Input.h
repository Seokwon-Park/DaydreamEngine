#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Daydream
{
	class Input
	{
	public:
		static bool GetKeyDown(KeyCode _key);
		static bool GetKeyPressed(KeyCode _key);
		static bool GetKeyUp(KeyCode _key);

		static bool GetMouseDown(MouseCode _button);
		static bool GetMousePressed(MouseCode _button);
		static bool GetMouseReleased(MouseCode _button);
		static Pair<Float32, Float32> GetMousePosition();
		static Float32 GetMouseX();
		static Float32 GetMouseY();
			
	};
}
