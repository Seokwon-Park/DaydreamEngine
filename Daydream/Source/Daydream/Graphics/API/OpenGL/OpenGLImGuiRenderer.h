#pragma once

#include "OpenGLRenderDevice.h"
#include "Daydream/ImGui/ImGuiRenderer.h"

namespace Daydream
{
	class OpenGLImGuiRenderer : public ImGuiRenderer
	{
	public:
		// constrcuter destructer
		OpenGLImGuiRenderer(OpenGLRenderDevice* _device);

		virtual void Init(DaydreamWindow* _window)override;
		virtual void Shutdown()override;
		virtual void NewFrame()override;
		virtual void Render(RenderCommandList* _activeCommandList)override;
	private:
		OpenGLRenderDevice* device;

	};
}

