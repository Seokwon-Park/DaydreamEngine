#pragma once


#include "Steins/Graphics/Resources/Framebuffer.h"
#include "Steins/Graphics/API/Vulkan/VulkanRenderDevice.h"

namespace Steins
{
	class VulkanSwapChain;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(VulkanRenderDevice* _device, const FramebufferDesc& _spec);
		VulkanFramebuffer(VulkanRenderDevice* _device, VulkanSwapChain* _swapChain);

		virtual ~VulkanFramebuffer() override;

		virtual void Begin() const override;
		virtual void End() const override;
		virtual void Clear(Color _color) override;

		VkFramebuffer GetFramebuffer() { return framebuffer; }
		VkRenderPass GetRenderPass() { return renderPass; }

	private:
		VulkanRenderDevice* device;
		VkRenderPass renderPass;
		VkFramebuffer framebuffer;
		VkExtent2D extent;
		Array<VkImage> colorImages;
		Array<VkImageView> colorImageViews;
		VkImage depthImage;
		VkImageView depthStencilViews;
	};
}