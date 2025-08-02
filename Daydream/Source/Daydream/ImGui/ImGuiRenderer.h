#pragma once
#include "Daydream/Core/Window.h"


namespace Daydream
{
	class Daydream_API ImGuiRenderer
	{
	public:
		virtual void Init(DaydreamWindow* _window);
		virtual void Shutdown();
		virtual void NewFrame();
		virtual void Render();
	private:
		DaydreamWindow* mainWindow = nullptr;
	};
}

