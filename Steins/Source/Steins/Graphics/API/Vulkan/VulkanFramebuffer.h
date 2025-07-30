#pragma once


#include "Steins/Graphics/Resources/Framebuffer.h"
#include "VulkanRenderDevice.h"
#include "VulkanTexture.h"
#include "VulkanRenderPass.h"

namespace Steins
{
	class VulkanSwapchain;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, const FramebufferDesc& _desc);
		VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, VulkanSwapchain* _swapChain, UInt32 _frameIndex);

		virtual ~VulkanFramebuffer() override;

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) override;
		virtual inline bool HasDepthAttachment() override { return depthAttachment != nullptr; }
		virtual void Resize(UInt32 _width, UInt32 _height) override;

		VkFramebuffer GetFramebuffer() { return framebuffer; }
		VkExtent2D GetExtent() { return extent; }

	private:
		VulkanRenderDevice* device;
		VkFramebuffer framebuffer;
		VkExtent2D extent;
		Array<VkImage> colorImages;
		Array<VkImageView> AttachmentImageViews;
		Array<Shared<VulkanTexture2D>> colorAttachments;
		Shared<VulkanTexture2D> depthAttachment;
		VkImageView depthStencilView;
	};
}