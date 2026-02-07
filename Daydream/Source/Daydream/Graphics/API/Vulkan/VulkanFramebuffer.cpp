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

		if (entityTexture)
		{
			vk::BufferCreateInfo bufferInfo{};
			bufferInfo.size = sizeof(uint32_t);
			bufferInfo.usage = vk::BufferUsageFlagBits::eTransferDst;

			vma::AllocationCreateInfo allocInfo{};
			allocInfo.usage = vma::MemoryUsage::eAuto;
			// [중요] CPU에서 Map하여 읽을 것이므로 Host Access 플래그 필수
			allocInfo.flags = vma::AllocationCreateFlagBits::eMapped | vma::AllocationCreateFlagBits::eHostAccessRandom;

			std::tie(stagingBuffer, stagingBufferAllocation) = device->CreateBuffer(bufferInfo, allocInfo);
		}
	}

	VulkanFramebuffer::VulkanFramebuffer(VulkanRenderDevice* _device, VulkanSwapchain* _swapchain, VulkanRenderPass* _renderPass, vk::Image _swapchainImage)
	{
		device = _device;
		extent = _swapchain->GetExtent();
		width = extent.width;
		height = extent.height;
		renderPass = _renderPass;
		vkRenderPass = _renderPass;

		swapchainImageView = device->CreateImageView(_swapchainImage,
			_swapchain->GetFormat(),
			vk::ImageAspectFlagBits::eColor);
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

	UInt32 VulkanFramebuffer::ReadEntityHandleFromPixel(Int32 _mouseX, Int32 _mouseY)
	{
		vk::Image srcImage = entityTexture->GetVkImage(); // Entity ID가 담긴 소스 이미지

		vk::CommandBuffer cmd = device->BeginSingleTimeCommands();

		// 3. 이미지 레이아웃 변경 (Barrier): ??? -> TRANSFER_SRC_OPTIMAL
		// 현재 레이아웃을 추적해야 하지만, 보통 COLOR_ATTACHMENT_OPTIMAL이거나 SHADER_READ_ONLY_OPTIMAL 일 것입니다.
		vk::ImageMemoryBarrier barrierToTransfer{};
		barrierToTransfer.oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal; // 혹은 추적된 현재 레이아웃
		barrierToTransfer.newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrierToTransfer.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		barrierToTransfer.dstAccessMask = vk::AccessFlagBits::eTransferRead;
		barrierToTransfer.image = srcImage;
		barrierToTransfer.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

		cmd.pipelineBarrier(
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eTransfer,
			vk::DependencyFlags{},
			nullptr, nullptr, barrierToTransfer // image barriers
		);

		vk::BufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource = { vk::ImageAspectFlagBits::eColor, 0, 0, 1 };
		region.imageOffset = vk::Offset3D(_mouseX, _mouseY, 0);
		region.imageExtent = vk::Extent3D(1, 1, 1);

		cmd.copyImageToBuffer(srcImage, vk::ImageLayout::eTransferSrcOptimal, stagingBuffer.get(), region);

		vk::ImageMemoryBarrier barrierToOriginal{};
		barrierToOriginal.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrierToOriginal.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrierToOriginal.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		barrierToOriginal.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		barrierToOriginal.image = srcImage;
		barrierToOriginal.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

		cmd.pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::DependencyFlags{},
			nullptr, nullptr, barrierToOriginal
		);

		device->EndSingleTimeCommands(cmd);

		UInt32 pixelValue = 0;
		vma::AllocationInfo allocationInfo;
		device->GetAllocator().getAllocationInfo(stagingBufferAllocation.get(), &allocationInfo);

		if (allocationInfo.pMappedData)
		{
			pixelValue = *static_cast<UInt32*>(allocationInfo.pMappedData);
		}

		return pixelValue;
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

			vk::ImageMemoryBarrier barrier{};
			barrier.oldLayout = vk::ImageLayout::eUndefined;
			barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = colorTexture->GetVkImage();
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.subresourceRange.levelCount = 1;

			device->TransitionImageLayout(barrier);

			if (colorAttachmentDesc.type == AttachmentType::EntityHandle)
			{
				entityTexture = colorTexture;
			}
			colorAttachments.push_back(colorTexture);
			attachmentImageViews.push_back(colorTexture->GetImageView());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil | RenderBindFlags::ShaderResource;

			Shared<VulkanTexture2D> depthTexture = MakeShared<VulkanTexture2D>(device, textureDesc);

			depthAttachment = depthTexture;
			depthStencilView = depthTexture->GetImageView();
			attachmentImageViews.push_back(depthAttachment->GetImageView());
		}

		vk::FramebufferCreateInfo framebufferInfo{};
		framebufferInfo.renderPass = vkRenderPass->GetVkRenderPass();
		framebufferInfo.attachmentCount = (UInt32)attachmentImageViews.size();
		framebufferInfo.pAttachments = attachmentImageViews.data();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		framebuffer = device->GetDevice().createFramebufferUnique(framebufferInfo);
	}
}