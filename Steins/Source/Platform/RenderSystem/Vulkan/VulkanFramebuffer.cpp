#include "SteinsPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"


namespace Steins
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanGraphicsDevice* _device, const FramebufferDesc& _spec)
	{
		device = _device;
	}
	VulkanFramebuffer::VulkanFramebuffer(VulkanGraphicsDevice* _device, VulkanSwapChain* _swapChain)
	{
		device = _device;

		UInt32 swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapChain->GetVKSwapChain(), &swapChainImageCount, nullptr);
		colorImages.resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapChain->GetVKSwapChain(), &swapChainImageCount, colorImages.data());

		colorImageViews.resize(swapChainImageCount);
		framebuffers.resize(swapChainImageCount);
		for (UInt32 i = 0; i < swapChainImageCount; i++)
		{
			device->CreateImageView(colorImages[i], VK_FORMAT_R8G8B8A8_UNORM, colorImageViews[i]);
		}

		for (size_t i = 0; i < colorImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				colorImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = device->GetMainRenderPass();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = _swapChain->GetExtent().width;
			framebufferInfo.height = _swapChain->GetExtent().height;
			framebufferInfo.layers = 1;

			VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffers[i]);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
		}

	}
	VulkanFramebuffer::~VulkanFramebuffer()
	{
		for (auto imageView : colorImageViews)
		{
			vkDestroyImageView(device->GetDevice(), imageView, nullptr);
		}
		for (auto framebuffer : framebuffers)
		{
			vkDestroyFramebuffer(device->GetDevice(), framebuffer, nullptr);
		}
	}
	void VulkanFramebuffer::Bind() const
	{
		//vkCmdBeginRenderPass(device->GetCommandBuffer(), );
	}
	void VulkanFramebuffer::Clear(Color _color)
	{
	}
}