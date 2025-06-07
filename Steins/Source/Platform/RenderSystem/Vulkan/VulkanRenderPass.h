#pragma once

#include "VulkanRenderDevice.h"
#include "Steins/Render/RenderPass.h"

namespace Steins
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(VulkanRenderDevice* _device);

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual void* GetNativeHandle() const { return renderPass; }
		VkRenderPass GetVkRenderPass() const { return renderPass; }

	private:
		VulkanRenderDevice* device;
		VkRenderPass renderPass;
	};
}