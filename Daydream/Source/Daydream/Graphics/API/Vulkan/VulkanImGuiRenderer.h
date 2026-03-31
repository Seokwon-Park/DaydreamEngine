#pragma once

#include "VulkanRenderDevice.h"
#include "Daydream/ImGui/ImGuiRenderer.h"

namespace Daydream
{
	class VulkanImGuiRenderer : public ImGuiRenderer
	{
	public:
		VulkanImGuiRenderer(VulkanRenderDevice* _device);

		virtual void Init(DaydreamWindow* _window)override;
		virtual void Shutdown()override;
		virtual void NewFrame()override;
		virtual void RenderDrawData(RenderCommandList* _activeCommandList, ImDrawData* _drawData)override;
	private:
		VulkanRenderDevice* device;
	};
}

