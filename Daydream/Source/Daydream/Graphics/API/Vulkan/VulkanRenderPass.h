#pragma once

#include "VulkanRenderDevice.h"
#include "Daydream/Graphics/Core/RenderPass.h"

namespace Daydream
{
	class VulkanFramebuffer;

	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(VulkanRenderDevice* _device, const RenderPassDesc& _desc);
		virtual ~VulkanRenderPass();

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) override;

		vk::RenderPass GetVkRenderPass() const { return renderPass.get(); }
	private:
		VulkanRenderDevice* device;
		vk::UniqueRenderPass renderPass;
		Shared<VulkanFramebuffer> currentFramebuffer;
	};
}