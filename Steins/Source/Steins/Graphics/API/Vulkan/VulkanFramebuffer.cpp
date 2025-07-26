#include "SteinsPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, const FramebufferDesc& _desc)
	{
		device = _device;

		CreateRenderPass(_desc, false);

		for (const auto& colorAttachmentDesc : _desc.colorAttachments)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<VulkanTexture2D> backBufferTexture = MakeShared<VulkanTexture2D>(device, textureDesc);
			colorAttachments.push_back(backBufferTexture);
			colorImageViews.push_back(backBufferTexture->GetImageView());
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = colorImageViews.size();
		framebufferInfo.pAttachments = colorImageViews.data();
		framebufferInfo.width = _desc.width;
		framebufferInfo.height = _desc.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffer);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");

	}
	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanSwapchain* _swapchain, UInt32 _frameIndex)
	{
		device = _device;
		extent = _swapchain->GetExtent();

		FramebufferDesc fbDesc;

		FramebufferAttachmentDesc attach{};
		attach.format = Steins::RenderFormat::R8G8B8A8_UNORM;
		attach.loadOp = Steins::AttachmentLoadOp::Clear;
		attach.storeOp = Steins::AttachmentStoreOp::Store;
		fbDesc.colorAttachments.push_back(attach);

		CreateRenderPass(fbDesc, true);
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
		framebufferInfo.renderPass = renderPass;
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
		if(renderPass != VK_NULL_HANDLE) vkDestroyRenderPass(device->GetDevice(), renderPass, nullptr);
		colorImageViews.clear();
		if(framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(device->GetDevice(), framebuffer, nullptr);
	}
	void VulkanFramebuffer::Begin() const
	{
		VkViewport viewport{};
		//viewport.x = 0.0f;
		//viewport.y = (float)extent.height;
		//viewport.width = (float)extent.width;
		//viewport.height = -(float)extent.height;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)extent.width;
		viewport.height = (float)extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(device->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;
		vkCmdSetScissor(device->GetCommandBuffer(), 0, 1, &scissor);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = extent;

		VkClearValue clearColor = { {0.0f, 0.0f, 0.0f, 1.0f} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(device->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		device->SetCurrentRenderPass(renderPass);
	}
	void VulkanFramebuffer::End() const
	{
		vkCmdEndRenderPass(device->GetCommandBuffer());
	}
	void VulkanFramebuffer::Clear(Color _color)
	{
	}

	void VulkanFramebuffer::CreateRenderPass(const FramebufferDesc& desc, bool _isBackBuffer)
	{
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkAttachmentReference> colorAttachmentRefs;

		// 컬러 attachment들
		for (size_t i = 0; i < desc.colorAttachments.size(); ++i)
		{
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = GraphicsUtil::RenderFormatToVkFormat(desc.colorAttachments[i].format);
			colorAttachment.samples = static_cast<VkSampleCountFlagBits>(desc.samples);
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = _isBackBuffer ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			attachments.push_back(colorAttachment);

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = static_cast<uint32_t>(i);
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorAttachmentRefs.push_back(colorAttachmentRef);
		}

		// 깊이 attachment
		VkAttachmentReference depthAttachmentRef{};
		bool hasDepth = (desc.depthAttachment.format != RenderFormat::UNKNOWN);
		if (hasDepth)
		{
			VkAttachmentDescription depthAttachment{};
			depthAttachment.format = GraphicsUtil::RenderFormatToVkFormat(desc.depthAttachment.format);
			depthAttachment.samples = static_cast<VkSampleCountFlagBits>(desc.samples);
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			attachments.push_back(depthAttachment);

			depthAttachmentRef.attachment = static_cast<uint32_t>(attachments.size() - 1);
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
		subpass.pColorAttachments = colorAttachmentRefs.data();
		subpass.pDepthStencilAttachment = hasDepth ? &depthAttachmentRef : nullptr;

		// 서브패스 의존성 (외부 -> 서브패스)
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult result = vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &renderPass);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");
	}
}