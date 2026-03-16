#include "DaydreamPCH.h"
#include "VulkanRenderContext.h"

#include "VulkanPipelineState.h"
#include "VulkanTexture.h"
#include "VulkanTextureCube.h"
#include "VulkanBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderCommandList.h"
#include "Daydream/Graphics/Resources/Mesh.h"


namespace Daydream
{
	VulkanRenderContext::VulkanRenderContext(VulkanRenderDevice* _device, UInt32 _framesInFlight)
	{
		device = _device;

		//commandBuffers.resize(_framesInFlight);
		//vk::CommandBufferAllocateInfo allocInfo{};
		//allocInfo.commandPool = device->GetCommandPool();
		//allocInfo.level = vk::CommandBufferLevel::ePrimary;
		//allocInfo.commandBufferCount = (UInt32)commandBuffers.size();

		//commandBuffers = device->GetDevice().allocateCommandBuffersUnique(allocInfo);
		//commandBufferIndex = 0;

		//vk::FenceCreateInfo fenceInfo{};
		//fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		//waitFences.resize(_framesInFlight);
		//for (UInt32 i = 0; i < _framesInFlight; i++)
		//{
		//	waitFences[i] = device->GetDevice().createFenceUnique(fenceInfo);
		//}
	}
	VulkanRenderContext::~VulkanRenderContext()
	{
		device->GetGraphicsQueue().waitIdle();
	}
	void VulkanRenderContext::BeginCommandList()
	{
		//active = commandBuffers[commandBufferIndex].get();
		//vk::Result result = device->GetDevice().waitForFences(1, &waitFences[commandBufferIndex].get(), VK_FALSE, UINT64_MAX);
		//result = device->GetDevice().resetFences(1, &waitFences[commandBufferIndex].get());

		//GetActiveCommandBuffer().reset({});

		//vk::CommandBufferBeginInfo beginInfo{};
		//GetActiveCommandBuffer().begin(beginInfo);
	}

	void VulkanRenderContext::EndCommandList()
	{
		/*GetActiveCommandBuffer().end();

		vk::SubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[commandBufferIndex].get();

		vk::Result result = device->GetGraphicsQueue().submit(1, &submitInfo, waitFences[commandBufferIndex].get());

		commandBufferIndex = (commandBufferIndex + 1) % 3;

		device->GetGraphicsQueue().waitIdle();*/
	}
	void VulkanRenderContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
	{
		vk::Viewport viewport{};
		//viewport.x = 0.0f;
		//viewport.y = (float)extent.height;
		//viewport.width = (float)extent.width;
		//viewport.height = -(float)extent.height;
		viewport.x = _x;
		viewport.y = _y;
		viewport.width = (float)_width;
		viewport.height = (float)_height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		GetActiveCommandBuffer().setViewport(0, 1, &viewport);

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D(_x, _y);
		scissor.extent = vk::Extent2D(_width, _height);
		GetActiveCommandBuffer().setScissor(0, 1, &scissor);
	}
	void VulkanRenderContext::SetClearColor(const Color& _color)
	{
	}

	void VulkanRenderContext::Clear()
	{
	}

	void VulkanRenderContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		GetActiveCommandBuffer().drawIndexed(_indexCount, 1, _startIndex, _baseVertex, 0);
	}
	void VulkanRenderContext::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
		Shared<VulkanFramebuffer> currentFramebuffer = static_pointer_cast<VulkanFramebuffer>(_framebuffer);
		Shared<VulkanRenderPass> renderPass = static_pointer_cast<VulkanRenderPass>(_renderPass);

		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo.renderPass = renderPass->GetVkRenderPass();
		renderPassInfo.framebuffer = currentFramebuffer->GetFramebuffer();
		renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
		renderPassInfo.renderArea.extent = currentFramebuffer->GetExtent();

		Array<vk::ClearValue> colors;
		for (UInt32 i = 0; i < _framebuffer->GetColorAttachmentSize(); i++)
		{
			vk::ClearValue vulkanClearColor;
			memcpy(vulkanClearColor.color.float32, _renderPass->GetClearColor().color, sizeof(Color));
			colors.push_back(vulkanClearColor);
		}

		if (currentFramebuffer->HasDepthAttachment())
		{
			vk::ClearValue vulkanClearDepthStencil;
			vulkanClearDepthStencil.depthStencil.depth = 1.0f; // 또는 0.0f
			vulkanClearDepthStencil.depthStencil.stencil = 0;   // 스텐실 값도 함께 초기화
			colors.push_back(vulkanClearDepthStencil);
		}

		renderPassInfo.clearValueCount = colors.size();
		renderPassInfo.pClearValues = colors.data();

		SetViewport(0, 0, currentFramebuffer->GetWidth(), currentFramebuffer->GetHeight());

		GetActiveCommandBuffer().beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	}
	void VulkanRenderContext::EndRenderPass(Shared<RenderPass> _renderPass)
	{
		GetActiveCommandBuffer().endRenderPass();
	}
	void VulkanRenderContext::BindPipelineState(Shared<PipelineState> _pipelineState)
	{
		RenderContext::BindPipelineState(_pipelineState);
		currentPipelineState = _pipelineState;
		Shared<VulkanPipelineState> pipelineState = static_pointer_cast<VulkanPipelineState>(_pipelineState);

		GetActiveCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineState->GetPipeline());
	}
	void VulkanRenderContext::BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		vk::DeviceSize offset = 0;
		Shared<VulkanVertexBuffer> vertexBuffer = SharedCast<VulkanVertexBuffer>(_vertexBuffer);
		GetActiveCommandBuffer().bindVertexBuffers(0, { vertexBuffer->GetVkBuffer() }, { offset });
	}
	void VulkanRenderContext::BindIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		Shared<VulkanIndexBuffer> indexBuffer = SharedCast<VulkanIndexBuffer>(_indexBuffer);
		GetActiveCommandBuffer().bindIndexBuffer(indexBuffer->GetVkBuffer(), 0, vk::IndexType::eUint32);
	}

	void VulkanRenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		RenderContext::SetTexture2D(_name, _texture);
		const ShaderReflectionData* resourceInfo = currentPipelineState->GetBindingInfo(_name);
		if (resourceInfo == nullptr) return;

		Shared<VulkanTexture2D> vulkanTexture = SharedCast<VulkanTexture2D>(_texture);
		Shared<VulkanPipelineState> vulkanPSO = SharedCast<VulkanPipelineState>(currentPipelineState);

		vk::DescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = vulkanTexture->GetImageView();
		imageInfo.sampler = vulkanTexture->GetSampler();

		vk::WriteDescriptorSet writeSet = {};
		//writeSet.dstSet = sets[resourceInfo.set].get();
		writeSet.dstBinding = resourceInfo->binding;  // 특정 binding만 업데이트
		writeSet.descriptorCount = 1;
		writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
		writeSet.pImageInfo = &imageInfo;

		GetActiveCommandBuffer().pushDescriptorSet(
			vk::PipelineBindPoint::eGraphics,
			vulkanPSO->GetPipelineLayout(),
			resourceInfo->set,
			1,
			&writeSet
		);
	}
	void VulkanRenderContext::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		const ShaderReflectionData* resourceInfo = currentPipelineState->GetBindingInfo(_name);
		if (resourceInfo == nullptr) return;

		Shared<VulkanTextureCube> vulkanTexture = std::static_pointer_cast<VulkanTextureCube>(_textureCube);
		Shared<VulkanPipelineState> vulkanPSO = std::static_pointer_cast<VulkanPipelineState>(currentPipelineState);

		vk::DescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = vulkanTexture->GetImageView();
		imageInfo.sampler = vulkanTexture->GetSampler();

		vk::WriteDescriptorSet writeSet = {};
		//writeSet.dstSet = sets[resourceInfo.set].get();
		writeSet.dstBinding = resourceInfo->binding;  // 특정 binding만 업데이트
		writeSet.descriptorCount = 1;
		writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
		writeSet.pImageInfo = &imageInfo;

		GetActiveCommandBuffer().pushDescriptorSet(
			vk::PipelineBindPoint::eGraphics,
			vulkanPSO->GetPipelineLayout(),
			resourceInfo->set,
			1,
			&writeSet
		);
	}
	void VulkanRenderContext::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		const ShaderReflectionData* resourceInfo = currentPipelineState->GetBindingInfo(_name);
		if (resourceInfo == nullptr) return;

		Shared<VulkanConstantBuffer> vulkanBuffer = std::static_pointer_cast<VulkanConstantBuffer>(_buffer);
		Shared<VulkanPipelineState> vulkanPSO = std::static_pointer_cast<VulkanPipelineState>(currentPipelineState);

		vk::DescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = (VkBuffer)vulkanBuffer->GetNativeHandle();
		bufferInfo.offset = 0;
		bufferInfo.range = vulkanBuffer->GetSize();

		vk::WriteDescriptorSet writeSet = {};
		//writeSet.dstSet = sets[resourceInfo.set].get();
		writeSet.dstBinding = resourceInfo->binding;  // 특정 binding만 업데이트
		writeSet.descriptorCount = 1;
		writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
		writeSet.pBufferInfo = &bufferInfo;

		GetActiveCommandBuffer().pushDescriptorSet(
			vk::PipelineBindPoint::eGraphics,
			vulkanPSO->GetPipelineLayout(),
			resourceInfo->set,
			1,
			&writeSet
		);
	}

	void VulkanRenderContext::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
		vk::ImageMemoryBarrier barriers[2] = {};

		Shared<VulkanTexture2D> dst = SharedCast<VulkanTexture2D>(_dst);
		Shared<VulkanTexture2D> src = SharedCast<VulkanTexture2D>(_src);

		// 원본 이미지를 TRANSFER_SRC로 변경
		barriers[0].oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal; // 또는 현재 레이아웃
		barriers[0].newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].image = src->GetVkImage();
		barriers[0].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[0].subresourceRange.baseMipLevel = 0;
		barriers[0].subresourceRange.levelCount = 1;
		barriers[0].subresourceRange.baseArrayLayer = 0;
		barriers[0].subresourceRange.layerCount = 1;
		barriers[0].srcAccessMask = {}; // 이전 작업이 없다고 가정
		barriers[0].dstAccessMask = vk::AccessFlagBits::eTransferRead;

		// 대상 이미지를 TRANSFER_DST로 변경
		barriers[1].oldLayout = vk::ImageLayout::eUndefined; // 또는 현재 레이아웃
		barriers[1].newLayout = vk::ImageLayout::eTransferDstOptimal;
		barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].image = dst->GetVkImage();;
		barriers[1].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[1].subresourceRange.baseMipLevel = 0;
		barriers[1].subresourceRange.levelCount = 1;
		barriers[1].subresourceRange.baseArrayLayer = 0;
		barriers[1].subresourceRange.layerCount = 1;
		barriers[1].srcAccessMask = {};
		barriers[1].dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		GetActiveCommandBuffer().pipelineBarrier(
			vk::PipelineStageFlagBits::eTopOfPipe,  // 이전 작업 단계
			vk::PipelineStageFlagBits::eTransfer,     // 다음 작업 단계 (전송)
			{},
			0, nullptr,
			0, nullptr,
			2, barriers
		);

		// 2. 복사 명령 기록
		vk::ImageCopy copyRegion = {};
		copyRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyRegion.srcSubresource.layerCount = 1;
		copyRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyRegion.dstSubresource.layerCount = 1;
		copyRegion.extent.width = _src->GetWidth();
		copyRegion.extent.height = _src->GetHeight();
		copyRegion.extent.depth = 1;

		GetActiveCommandBuffer().copyImage(
			src->GetVkImage(), vk::ImageLayout::eTransferSrcOptimal,
			dst->GetVkImage(), vk::ImageLayout::eTransferDstOptimal,
			1, &copyRegion
		);


		// 원본 이미지를 TRANSFER_SRC로 변경
		barriers[0].oldLayout = vk::ImageLayout::eTransferSrcOptimal; // 또는 현재 레이아웃
		barriers[0].newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].image = src->GetVkImage();
		barriers[0].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[0].subresourceRange.baseMipLevel = 0;
		barriers[0].subresourceRange.levelCount = 1;
		barriers[0].subresourceRange.baseArrayLayer = 0;
		barriers[0].subresourceRange.layerCount = 1;
		barriers[0].srcAccessMask = vk::AccessFlagBits::eTransferRead; // 이전 작업이 없다고 가정
		barriers[0].dstAccessMask = {};

		// 대상 이미지를 TRANSFER_DST로 변경
		barriers[1].oldLayout = vk::ImageLayout::eTransferDstOptimal; // 또는 현재 레이아웃
		barriers[1].newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].image = dst->GetVkImage();;
		barriers[1].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[1].subresourceRange.baseMipLevel = 0;
		barriers[1].subresourceRange.levelCount = 1;
		barriers[1].subresourceRange.baseArrayLayer = 0;
		barriers[1].subresourceRange.layerCount = 1;
		barriers[1].srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barriers[1].dstAccessMask = {};

		GetActiveCommandBuffer().pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer,  // 이전 작업 단계
			vk::PipelineStageFlagBits::eFragmentShader,     // 다음 작업 단계 (전송)
			{},
			0, nullptr,
			0, nullptr,
			2, barriers
		);
	}
	void VulkanRenderContext::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
		Shared<VulkanTextureCube> dst = SharedCast<VulkanTextureCube>(_dstCubemap);
		Shared<VulkanTexture2D> src = SharedCast<VulkanTexture2D>(_srcTexture2D);

		vk::ImageMemoryBarrier barriers[2] = {};

		// 원본 이미지를 TRANSFER_SRC로 변경
		barriers[0].oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal; // 또는 현재 레이아웃
		barriers[0].newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].image = src->GetVkImage();
		barriers[0].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[0].subresourceRange.baseMipLevel = 0;
		barriers[0].subresourceRange.levelCount = 1;
		barriers[0].subresourceRange.baseArrayLayer = 0;
		barriers[0].subresourceRange.layerCount = 1;
		barriers[0].srcAccessMask = {}; // 이전 작업이 없다고 가정
		barriers[0].dstAccessMask = vk::AccessFlagBits::eTransferRead;

		// 대상 이미지를 TRANSFER_DST로 변경
		barriers[1].oldLayout = vk::ImageLayout::eUndefined; // 또는 현재 레이아웃
		barriers[1].newLayout = vk::ImageLayout::eTransferDstOptimal;
		barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].image = dst->GetVkImage();;
		barriers[1].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[1].subresourceRange.baseMipLevel = _mipLevel;
		barriers[1].subresourceRange.levelCount = 1;
		barriers[1].subresourceRange.baseArrayLayer = _faceIndex;
		barriers[1].subresourceRange.layerCount = 1;
		barriers[1].srcAccessMask = {};
		barriers[1].dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		GetActiveCommandBuffer().pipelineBarrier(
			vk::PipelineStageFlagBits::eTopOfPipe,  // 이전 작업 단계
			vk::PipelineStageFlagBits::eTransfer,     // 다음 작업 단계 (전송)
			{},
			0, nullptr,
			0, nullptr,
			2, barriers
		);

		//barrier.oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		//barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
		//barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		//barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		//barrier.image = dst->GetVkImage();->GetImage();
		//barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		//barrier.subresourceRange.baseArrayLayer = _faceIndex;
		//barrier.subresourceRange.baseMipLevel = 0;
		//barrier.subresourceRange.layerCount = 1;
		//barrier.subresourceRange.levelCount = 1;

		//TransitionImageLayout(barrier);

		// --- 3. vkCmdCopyImage 명령 기록 ---
		vk::ImageCopy copyRegion{};

		// 소스 서브리소스 설정
		copyRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyRegion.srcSubresource.mipLevel = 0;
		copyRegion.srcSubresource.baseArrayLayer = 0; // 2D 텍스처이므로 0
		copyRegion.srcSubresource.layerCount = 1;
		copyRegion.srcOffset = vk::Offset3D{ 0, 0, 0 };

		// 목적지 서브리소스 설정
		copyRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		copyRegion.dstSubresource.mipLevel = _mipLevel;
		copyRegion.dstSubresource.baseArrayLayer = _faceIndex; // 큐브맵의 특정 면을 가리킴
		copyRegion.dstSubresource.layerCount = 1;
		copyRegion.dstOffset = vk::Offset3D{ 0, 0, 0 };

		// 복사할 영역의 크기
		copyRegion.extent.width = _srcTexture2D->GetWidth();
		copyRegion.extent.height = _srcTexture2D->GetHeight();
		copyRegion.extent.depth = 1;

		GetActiveCommandBuffer().copyImage(
			src->GetVkImage(), vk::ImageLayout::eTransferSrcOptimal,
			dst->GetVkImage(), vk::ImageLayout::eTransferDstOptimal,
			1, &copyRegion
		);

		barriers[0].oldLayout = vk::ImageLayout::eTransferSrcOptimal; // 또는 현재 레이아웃
		barriers[0].newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[0].image = src->GetVkImage();
		barriers[0].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[0].subresourceRange.baseMipLevel = 0;
		barriers[0].subresourceRange.levelCount = 1;
		barriers[0].subresourceRange.baseArrayLayer = 0;
		barriers[0].subresourceRange.layerCount = 1;
		barriers[0].srcAccessMask = vk::AccessFlagBits::eTransferRead; // 이전 작업이 없다고 가정
		barriers[0].dstAccessMask = {};

		barriers[1].oldLayout = vk::ImageLayout::eTransferDstOptimal; // 또는 현재 레이아웃
		barriers[1].newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barriers[1].image = dst->GetVkImage();
		barriers[1].subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barriers[1].subresourceRange.baseMipLevel = _mipLevel;
		barriers[1].subresourceRange.levelCount = 1;
		barriers[1].subresourceRange.baseArrayLayer = _faceIndex;
		barriers[1].subresourceRange.layerCount = 1;
		barriers[1].srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barriers[1].dstAccessMask = {};

		GetActiveCommandBuffer().pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer,  // 이전 작업 단계
			vk::PipelineStageFlagBits::eFragmentShader,     // 다음 작업 단계 (전송)
			{},
			0, nullptr,
			0, nullptr,
			2, barriers
		);
	}

	void VulkanRenderContext::GenerateMips(Shared<Texture> _texture)
	{
		//vk::CommandBuffer commandBuffer = device->BeginSingleTimeCommands(); // 이 함수는 vk::CommandBuffer를 반환한다고 가정

		Int32 mipWidth = _texture->GetWidth();
		Int32 mipHeight = _texture->GetHeight();
		UInt32 layerCount = _texture->GetLayerCount();
		UInt32 mipLevels = _texture->GetMipLevels();
		vk::Image image;

		switch (_texture->GetType())
		{
		case TextureType::Texture2D:
			image = SharedCast<VulkanTexture2D>(_texture)->GetVkImage();
			break;
		case TextureType::TextureCube:
			image = SharedCast<VulkanTextureCube>(_texture)->GetVkImage();
			break;
		}

		// vk::ImageMemoryBarrier 구조체 사용, sType은 자동 설정됩니다.
		vk::ImageMemoryBarrier barrier{};
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // 일부 매크로는 그대로 사용
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		// commandBuffer의 멤버 함수 pipelineBarrier 호출
		GetActiveCommandBuffer().pipelineBarrier(
			vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eTransfer,
			{}, // vk::DependencyFlags
			nullptr, // MemoryBarriers
			nullptr, // BufferMemoryBarriers
			barrier  // ImageMemoryBarrier
		);



		for (UInt32 i = 1; i < _texture->GetMipLevels(); i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.subresourceRange.levelCount = 1;
			barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
			barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

			// commandBuffer의 멤버 함수 pipelineBarrier 호출
			GetActiveCommandBuffer().pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
				{}, // vk::DependencyFlags
				nullptr, // MemoryBarriers
				nullptr, // BufferMemoryBarriers
				barrier  // ImageMemoryBarrier
			);

			vk::ImageBlit blit{};
			blit.srcOffsets[0] = vk::Offset3D{ 0, 0, 0 };
			blit.srcOffsets[1] = vk::Offset3D{ mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = layerCount;
			blit.dstOffsets[0] = vk::Offset3D{ 0, 0, 0 };
			blit.dstOffsets[1] = vk::Offset3D{ mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = layerCount;

			// commandBuffer의 멤버 함수 blitImage 호출
			// 배열이 아닌 단일 blit 객체를 직접 전달합니다.
			GetActiveCommandBuffer().blitImage(
				image, vk::ImageLayout::eTransferSrcOptimal,
				image, vk::ImageLayout::eTransferDstOptimal,
				1, &blit,
				vk::Filter::eLinear
			);

			// 이전 루프에서 SRC로 바꾼 레이아웃을 최종 목적인 SHADER_READ_ONLY로 변경
			barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
			barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

			GetActiveCommandBuffer().pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
				{}, // vk::DependencyFlags
				nullptr,
				nullptr,
				barrier
			);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // 일부 매크로는 그대로 사용
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		// commandBuffer의 멤버 함수 pipelineBarrier 호출
		GetActiveCommandBuffer().pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader,
			{}, // vk::DependencyFlags
			nullptr, // MemoryBarriers
			nullptr, // BufferMemoryBarriers
			barrier  // ImageMemoryBarrier
		);
	}
	void VulkanRenderContext::SetCommandList(RenderCommandList* _cmd)
	{
		active = Cast<VulkanRenderCommandList*>(_cmd)->GetVkCommandBuffer();;
	}
	vk::CommandBuffer VulkanRenderContext::GetActiveCommandBuffer()
	{
		return active;
	}
}
