#pragma once


#include "Steins/Render/Framebuffer.h"
#include "Platform/RenderSystem/Vulkan/VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanSwapChain;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(VulkanGraphicsDevice* _device, const FramebufferDesc& _spec);
		VulkanFramebuffer(VulkanGraphicsDevice* _device, VulkanSwapChain* _swapChain);

		virtual ~VulkanFramebuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Clear(Color _color) override;

		VkFramebuffer GetFramebuffer() { return framebuffer; }
		VkRenderPass GetRenderPass() { return renderPass; }

	private:
		VulkanGraphicsDevice* device;
		VkRenderPass renderPass;
		VkFramebuffer framebuffer;
		VkExtent2D extent;
		std::vector<VkImage> colorImages;
		std::vector<VkImageView> colorImageViews;
		VkImage depthImage;
		VkImageView depthStencilViews;
	};
}