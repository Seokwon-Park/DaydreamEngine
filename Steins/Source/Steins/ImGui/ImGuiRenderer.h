#pragma once
#include "Steins/Core/Window.h"


namespace Steins
{
	class STEINS_API ImGuiRenderer
	{
	public:
		virtual void Init(SteinsWindow* _window);
		virtual void Shutdown();
		virtual void NewFrame();
		virtual void Render();
	private:
		SteinsWindow* mainWindow = nullptr;
	};
}

