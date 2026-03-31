#pragma once
#include "Daydream/Core/Window.h"
#include "Daydream/Graphics/Core/RenderCommandList.h"

struct ImDrawData;

namespace Daydream
{
	class DAYDREAM_API ImGuiRenderer
	{
	public:
		virtual void Init(DaydreamWindow* _window);
		virtual void Shutdown();
		virtual void NewFrame();
		virtual void RenderDrawData(RenderCommandList* _activeCommandList, ImDrawData* _drawData);
	private:
		DaydreamWindow* mainWindow = nullptr;
	};
}

