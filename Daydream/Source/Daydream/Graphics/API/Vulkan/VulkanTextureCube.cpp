#include "DaydreamPCH.h"
#include "VulkanTextureCube.h"

#include "VulkanUtility.h"

namespace Daydream
{
	VulkanTextureCube::VulkanTextureCube(VulkanRenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;
		desc = _desc;

		width = _desc.width;
		height = _desc.height;

        mipLevels = _desc.mipLevels;

		textures.resize(6);

		imageFormat = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desc.format);

		vk::ImageCreateInfo imageInfo{};
		vma::AllocationCreateInfo allocInfo{};

		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = static_cast<UInt32>(width);
		imageInfo.extent.height = static_cast<UInt32>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 6;
		imageInfo.format = imageFormat;
		imageInfo.tiling = vk::ImageTiling::eOptimal;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		imageInfo.usage = GraphicsUtility::Vulkan::ConvertToVkImageUsageFlags(_desc.bindFlags);
		imageInfo.sharingMode = vk::SharingMode::eExclusive;
		imageInfo.samples = vk::SampleCountFlagBits::e1;
		imageInfo.flags = vk::ImageCreateFlagBits::eCubeCompatible;

		allocInfo.usage = vma::MemoryUsage::eGpuOnly;
		allocInfo.flags = {};

		std::tie(textureImage, textureImageAllocation) = device->CreateImage(imageInfo, allocInfo);

		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = vk::ImageLayout::eUndefined;
		barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = textureImage.get();
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.layerCount = 6;
		barrier.subresourceRange.levelCount = mipLevels;

		device->TransitionImageLayout(barrier);

		vk::ImageViewCreateInfo viewInfo{};
		viewInfo.image = textureImage.get();
		viewInfo.viewType = vk::ImageViewType::eCube;
		viewInfo.format = imageFormat;
		viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 6;

		textureImageView = device->CreateImageView(viewInfo);
	}
	VulkanTextureCube::~VulkanTextureCube()
	{
	}

	void VulkanTextureCube::SetSampler(Shared<Sampler> _sampler)
	{
		textureSampler = static_cast<VulkanSampler*>(_sampler.get());
	}
	void VulkanTextureCube::GenerateMips()
	{
        // vk::PhysicalDevice의 멤버 함수를 사용해 format 속성을 가져옵니다.
        vk::FormatProperties formatProperties = device->GetPhysicalDevice().getFormatProperties(imageFormat);

        // 열거형을 vk::FormatFeatureFlagBits 타입으로 안전하게 비교합니다.
        if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
            throw std::runtime_error("texture image format does not support linear blitting!");
        }

        //vk::CommandBuffer commandBuffer = device->BeginSingleTimeCommands(); // 이 함수는 vk::CommandBuffer를 반환한다고 가정
        vk::CommandBuffer commandBuffer = device->GetCommandBuffer();

        vk::Image image = textureImage.get();
        // vk::ImageMemoryBarrier 구조체 사용, sType은 자동 설정됩니다.
        vk::ImageMemoryBarrier barrier{};
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // 일부 매크로는 그대로 사용
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 6;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        // commandBuffer의 멤버 함수 pipelineBarrier 호출
        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eTransfer,
            {}, // vk::DependencyFlags
            nullptr, // MemoryBarriers
            nullptr, // BufferMemoryBarriers
            barrier  // ImageMemoryBarrier
        );


        Int32 mipWidth = width;
        Int32 mipHeight = height;

        for (uint32_t i = 1; i < mipLevels; i++) 
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.subresourceRange.levelCount = 1;
            barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

            // commandBuffer의 멤버 함수 pipelineBarrier 호출
            commandBuffer.pipelineBarrier(
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
            blit.srcSubresource.layerCount = 6;
            blit.dstOffsets[0] = vk::Offset3D{ 0, 0, 0 };
            blit.dstOffsets[1] = vk::Offset3D{ mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 6;

            // commandBuffer의 멤버 함수 blitImage 호출
            // 배열이 아닌 단일 blit 객체를 직접 전달합니다.
            commandBuffer.blitImage(
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

            commandBuffer.pipelineBarrier(
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
        barrier.subresourceRange.layerCount = 6;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        // commandBuffer의 멤버 함수 pipelineBarrier 호출
        commandBuffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader,
            {}, // vk::DependencyFlags
            nullptr, // MemoryBarriers
            nullptr, // BufferMemoryBarriers
            barrier  // ImageMemoryBarrier
        );
        // 루프에서 처리되지 않은 마지막 Mipmap 레벨의 레이아웃을 변경
        //barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        //barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        //barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
        //barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        //barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        //commandBuffer.pipelineBarrier(
        //    vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader,
        //    {}, // vk::DependencyFlags
        //    nullptr,
        //    nullptr,
        //    barrier
        //);

        //device->EndSingleTimeCommands(commandBuffer);
	}
}
