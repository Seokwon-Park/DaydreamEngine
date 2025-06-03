#pragma once

#include "VulkanGraphicsDevice.h"
#include "Steins/Render/RenderPass.h"

namespace Steins
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(VulkanGraphicsDevice* _device);

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual void* GetNativeHandle() const { return renderPass; }
		VkRenderPass GetVkRenderPass() const { return renderPass; }

	private:
		VulkanGraphicsDevice* device;
		VkRenderPass renderPass;
	};
}