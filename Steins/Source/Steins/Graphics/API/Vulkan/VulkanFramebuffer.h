#pragma once


#include "Steins/Graphics/Resources/Framebuffer.h"
#include "VulkanRenderDevice.h"
#include "VulkanTexture.h"

namespace Steins
{
	class VulkanSwapchain;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(VulkanRenderDevice* _device, const FramebufferDesc& _desc);
		VulkanFramebuffer(VulkanRenderDevice* _device, VulkanSwapchain* _swapChain, UInt32 _frameIndex);

		virtual ~VulkanFramebuffer() override;

		virtual void Begin() const override;
		virtual void End() const override;
		virtual void Clear(Color _color) override;

		VkFramebuffer GetFramebuffer() { return framebuffer; }
		VkRenderPass GetRenderPass() { return renderPass; }

	private:
		void CreateRenderPass(const FramebufferDesc& _desc, bool _isBackBuffer);


		VulkanRenderDevice* device;
		VkRenderPass renderPass;
		VkFramebuffer framebuffer;
		VkExtent2D extent;
		Array<VkImage> colorImages;
		Array<VkImageView> colorImageViews;
		Array<Shared<VulkanTexture2D>> colorAttachments;
		VkImage depthImage;
		VkImageView depthStencilViews;
	};
}