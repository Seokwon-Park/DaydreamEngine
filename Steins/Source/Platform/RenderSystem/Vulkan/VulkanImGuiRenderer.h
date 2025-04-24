#pragma once

#include "VulkanGraphicsDevice.h"
#include "VulkanSwapChain.h"
#include "Steins/ImGui/ImGuiRenderer.h"

namespace Steins
{
	class VulkanImGuiRenderer : public ImGuiRenderer
	{
	public:
		VulkanImGuiRenderer(VulkanGraphicsDevice* _device);

		virtual void Init(SteinsWindow* _window)override;
		virtual void Shutdown()override;
		virtual void NewFrame()override;
		virtual void Render()override;
	private:
		VulkanGraphicsDevice* device;
		VulkanSwapChain* swapChain;
	};
}

