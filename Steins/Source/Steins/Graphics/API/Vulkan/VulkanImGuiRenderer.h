#pragma once

#include "VulkanRenderDevice.h"
#include "Steins/ImGui/ImGuiRenderer.h"

namespace Steins
{
	class VulkanImGuiRenderer : public ImGuiRenderer
	{
	public:
		VulkanImGuiRenderer(VulkanRenderDevice* _device);

		virtual void Init(SteinsWindow* _window)override;
		virtual void Shutdown()override;
		virtual void NewFrame()override;
		virtual void Render()override;
	private:
		VulkanRenderDevice* device;
	};
}

