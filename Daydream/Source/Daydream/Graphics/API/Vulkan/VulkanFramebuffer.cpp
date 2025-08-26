#include "DaydreamPCH.h"
#include "VulkanFramebuffer.h"
#include "VulkanSwapchain.h"

namespace Daydream
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanRenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;
		extent = vk::Extent2D(_desc.width, _desc.height);
		width = _desc.width;
		height = _desc.height;
		renderPass = _renderPass;
		vkRenderPass = _renderPass;

		CreateAttachments();
	}

	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanSwapchain* _swapchain, VulkanRenderPass* _renderPass, vk::Image _swapchainImage)
	{
		device = _device;
		extent = _swapchain->GetExtent();
		width = extent.width;
		height = extent.height;
		renderPass = _renderPass;
		vkRenderPass = _renderPass;

		swapchainImageView = vk::UniqueImageView(device->CreateImageView(_swapchainImage, 
			_swapchain->GetFormat(), 
			vk::ImageAspectFlagBits::eColor),
			vk::detail::ObjectDestroy<vk::Device, vk::detail::DispatchLoaderDynamic>(device->GetDevice()));
		attachmentImageViews.push_back(swapchainImageView.get());

		CreateAttachments();
	}
	
	VulkanFramebuffer::~VulkanFramebuffer()
	{
		colorAttachments.clear();
	}

	Shared<Texture2D> VulkanFramebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index];
	}

	void VulkanFramebuffer::Resize(UInt32 _width, UInt32 _height)
	{
		vkDeviceWaitIdle(device->GetDevice());

		width = _width;
		height = _height;
		extent.width = _width;
		extent.height = _height;

		attachmentImageViews.clear();
		oldAttachments.clear();
		oldAttachments = std::move(colorAttachments);
		colorAttachments.clear();
		oldAttachments.push_back(depthAttachment);
		depthAttachment = nullptr;
		depthStencilView = VK_NULL_HANDLE;

		vk::CommandBufferBeginInfo beginInfo{};
		device->GetCommandBuffer().reset({});
		device->GetCommandBuffer().begin(beginInfo);
		CreateAttachments();
	}

	void VulkanFramebuffer::CreateAttachments()
	{
		const RenderPassDesc& renderPassDesc = renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			if (colorAttachmentDesc.isSwapchain) continue;
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<VulkanTexture2D> colorTexture = MakeShared<VulkanTexture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			attachmentImageViews.push_back(colorTexture->GetImageView());
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
			attachmentImageViews.push_back(depthAttachment->GetImageView());
		}

		vk::FramebufferCreateInfo framebufferInfo{};
		framebufferInfo.renderPass = vkRenderPass->GetVkRenderPass();
		framebufferInfo.attachmentCount = attachmentImageViews.size();
		framebufferInfo.pAttachments = attachmentImageViews.data();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		framebuffer = device->GetDevice().createFramebufferUnique(framebufferInfo);
	}
}