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
		extent = _swapChain->GetExtent();

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VK_FORMAT_R8G8B8A8_UNORM;
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

		VkResult result = vkCreateRenderPass(_device->GetDevice(), &renderPassInfo, nullptr, &renderPass);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");

		UInt32 swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapChain->GetVKSwapChain(), &swapChainImageCount, nullptr);
		colorImages.resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapChain->GetVKSwapChain(), &swapChainImageCount, colorImages.data());

		colorImageViews.resize(1);
		device->CreateImageView(colorImages[_swapChain->GetBackbufferIndex()], VK_FORMAT_R8G8B8A8_UNORM, colorImageViews[0]);
		
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = colorImageViews.size();
		framebufferInfo.pAttachments = colorImageViews.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
	}
	VulkanFramebuffer::~VulkanFramebuffer()
	{
		vkDestroyRenderPass(device->GetDevice(), renderPass, nullptr);
		for (auto imageView : colorImageViews)
		{
			vkDestroyImageView(device->GetDevice(), imageView, nullptr);
		}
		vkDestroyFramebuffer(device->GetDevice(), framebuffer, nullptr);
	}
	void VulkanFramebuffer::Bind() const
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = extent;

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(device->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		device->SetCurrentRenderPass(renderPass);
	}
	void VulkanFramebuffer::Unbind() const
	{
		vkCmdEndRenderPass(device->GetCommandBuffer());
	}
	void VulkanFramebuffer::Clear(Color _color)
	{
	}
}