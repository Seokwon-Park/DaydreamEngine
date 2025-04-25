#pragma once

#include "OpenGLGraphicsDevice.h"
#include "Steins/ImGui/ImGuiRenderer.h"

namespace Steins
{
	class OpenGLImGuiRenderer : public ImGuiRenderer
	{
	public:
		// constrcuter destructer
		OpenGLImGuiRenderer(OpenGLGraphicsDevice* _device);

		virtual void Init(SteinsWindow* _window)override;
		virtual void Shutdown()override;
		virtual void NewFrame()override;
		virtual void Render()override;
	private:
		OpenGLGraphicsDevice* device;

	};
}

