#include "SteinsPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"


namespace Steins
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanGraphicsDevice* _device, const FramebufferSpecification& _spec)
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
			VkImageViewCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = colorImages[i];
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = _swapChain->GetFormat();
			info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(device->GetDevice(), &info, nullptr, &colorImageViews[i]);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create image view!");
		}

		{
			//CreataRenderPass
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = _swapChain->GetFormat();
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;

			VkResult result = vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &renderPass);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");
		}

		for (size_t i = 0; i < colorImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				colorImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
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