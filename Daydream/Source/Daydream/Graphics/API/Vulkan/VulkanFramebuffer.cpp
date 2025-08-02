#include "DaydreamPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"

#include "Daydream/Graphics/Utility/GraphicsUtil.h"

namespace Daydream
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;
		extent.width = _desc.width;
		extent.height = _desc.height;
		renderPass = _renderPass;
		vkRenderPass = _renderPass;

		CreateAttachments();
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
		Daydream_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");*/

		UInt32 swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapchain->GetVKSwapchain(), &swapChainImageCount, nullptr);
		colorImages.resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(device->GetDevice(), _swapchain->GetVKSwapchain(), &swapChainImageCount, colorImages.data());

		Shared<VulkanTexture2D> backBufferTexture = MakeShared<VulkanTexture2D>(device, colorImages[_frameIndex], _swapchain->GetFormat());
		colorAttachments.push_back(backBufferTexture);
		AttachmentImageViews.push_back(backBufferTexture->GetImageView());
		//device->CreateImageView(colorImages[_frameIndex], _swapchain->GetFormat(), colorImageViews[0]);

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass->GetVkRenderPass();
		framebufferInfo.attachmentCount = AttachmentImageViews.size();
		framebufferInfo.pAttachments = AttachmentImageViews.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffer);
		Daydream_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
	}
	VulkanFramebuffer::~VulkanFramebuffer()
	{
		colorAttachments.clear();
		if (framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(device->GetDevice(), framebuffer, nullptr);
	}

	Shared<Texture2D> VulkanFramebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index];
	}

	void VulkanFramebuffer::Resize(UInt32 _width, UInt32 _height)
	{
		vkDeviceWaitIdle(device->GetDevice());

		AttachmentImageViews.clear();
		oldAttachments.clear();
		oldAttachments = std::move(colorAttachments);
		colorAttachments.clear();
		oldAttachments.push_back(depthAttachment);
		depthAttachment = nullptr;
		depthStencilView = VK_NULL_HANDLE;
		if (framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(device->GetDevice(), framebuffer, nullptr);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkResetCommandBuffer(device->GetCommandBuffer(), 0);
		vkBeginCommandBuffer(device->GetCommandBuffer(), &beginInfo);
		CreateAttachments();
	}

	void VulkanFramebuffer::CreateAttachments()
	{
		const RenderPassDesc& renderPassDesc = renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<VulkanTexture2D> colorTexture = MakeShared<VulkanTexture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			AttachmentImageViews.push_back(colorTexture->GetImageView());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil;

			Shared<VulkanTexture2D> depthTexture = MakeShared<VulkanTexture2D>(device, textureDesc);
			depthAttachment = depthTexture;
			depthStencilView = depthTexture->GetImageView();
			AttachmentImageViews.push_back(depthAttachment->GetImageView());
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = vkRenderPass->GetVkRenderPass();
		framebufferInfo.attachmentCount = AttachmentImageViews.size();
		framebufferInfo.pAttachments = AttachmentImageViews.data();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &framebuffer);
		Daydream_CORE_ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");

	}
}