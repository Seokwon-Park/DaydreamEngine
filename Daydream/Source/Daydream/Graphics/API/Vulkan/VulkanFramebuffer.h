#pragma once


#include "Daydream/Graphics/Resources/Framebuffer.h"
#include "VulkanRenderDevice.h"
#include "VulkanTexture.h"
#include "VulkanRenderPass.h"

namespace Daydream
{
	class VulkanSwapchain;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, const FramebufferDesc& _desc);
		VulkanFramebuffer(VulkanRenderDevice* _device, VulkanSwapchain* _swapchain, VulkanRenderPass* _renderPass, vk::Image _swapchainImage);

		virtual ~VulkanFramebuffer() override;

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) override;
		virtual Shared<Texture2D> GetDepthAttachmentTexture() override { return depthAttachment; };
		virtual inline bool HasDepthAttachment() override { return depthAttachment != nullptr; }
		virtual void Resize(UInt32 _width, UInt32 _height) override;
		virtual UInt32 ReadEntityHandleFromPixel(Int32 _mouseX, Int32 _mouseY) override;

		vk::Framebuffer GetFramebuffer() { return framebuffer.get(); }
		vk::Extent2D GetExtent() { return extent; }
		void CreateAttachments();

	private:
		VulkanRenderDevice* device;
		VulkanRenderPass* vkRenderPass;
		vk::UniqueFramebuffer framebuffer;
		vk::Extent2D extent;
		vk::UniqueImageView swapchainImageView;
		Array<vk::ImageView> attachmentImageViews;
		vk::ImageView depthStencilView;
		Array<Shared<VulkanTexture2D>> colorAttachments;
		Shared<VulkanTexture2D> depthAttachment;

		Array<Shared<VulkanTexture2D>> oldAttachments;

		Shared<VulkanTexture2D> entityTexture;
		vma::UniqueBuffer stagingBuffer;
		vma::UniqueAllocation stagingBufferAllocation;

	};
}