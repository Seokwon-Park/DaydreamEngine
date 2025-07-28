#pragma once

#include "VulkanRenderDevice.h"
#include "Steins/Graphics/Core/RenderPass.h"

namespace Steins
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(VulkanRenderDevice* _device, const RenderPassDesc& _desc);
		virtual ~VulkanRenderPass();

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) override;

		virtual void* GetNativeHandle() const { return renderPass; }
		VkRenderPass GetVkRenderPass() const { return renderPass; }
	private:
		VulkanRenderDevice* device;
		VkRenderPass renderPass;
	};
}