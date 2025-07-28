#include "SteinsPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;
		extent.width = _desc.width;
		extent.height = _desc.height;
		const RenderPassDesc& renderPassDesc = _renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<VulkanTexture2D> colorTexture = MakeShared<VulkanTexture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			colorImageViews.push_back(colorTexture->GetImageView());
		}

		if(renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil;

			Shared<VulkanTexture2D> depthTexture = MakeShared<VulkanTexture2D>(device, textureDesc);
			depthAttachment = depthTexture;
			depthStencilView = depthTexture->GetImageView();
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass->GetVkRenderPass();
		framebufferInfo.attachmentCount = colorImageViews.size();
		framebufferInfo.pAttachments = colorImageViews.data();
		framebufferInfo.width = _desc.width;
		framebufferInfo.height = _desc.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");

	}
	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, VulkanSwapchain* _swapchain, UInt32 _frameIndex)
	{
		device = _device;
		extent = _swapchain->GetExtent();

		/*	VkAttachmentDescription colorAttachment{};
		colorAttachment.format = _swapchain->GetFormat();
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
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");*/

		UInt32 swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapchain->GetVKSwapchain(), &swapChainImageCount, nullptr);
		colorImages.resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapchain->GetVKSwapchain(), &swapChainImageCount, colorImages.data());

		Shared<VulkanTexture2D> backBufferTexture = MakeShared<VulkanTexture2D>(device, colorImages[_frameIndex]);
		colorAttachments.push_back(backBufferTexture);
		colorImageViews.push_back(backBufferTexture->GetImageView());
		//device->CreateImageView(colorImages[_frameIndex], _swapchain->GetFormat(), colorImageViews[0]);
		
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass->GetVkRenderPass();
		framebufferInfo.attachmentCount = colorImageViews.size();
		framebufferInfo.pAttachments = colorImageViews.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
	} 
	VulkanFramebuffer::~VulkanFramebuffer()
	{
		colorImageViews.clear();
		if(framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(device->GetDevice(), framebuffer, nullptr);
	}

	void VulkanFramebuffer::Clear(Color _color)
	{
	}

	void* Steins::VulkanFramebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index]->GetImGuiHandle();
	}

}