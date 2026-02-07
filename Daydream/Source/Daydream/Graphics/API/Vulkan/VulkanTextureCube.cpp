#include "DaydreamPCH.h"
#include "VulkanTextureCube.h"

#include "VulkanUtility.h"

namespace Daydream
{
	VulkanTextureCube::VulkanTextureCube(VulkanRenderDevice* _device, const TextureDesc& _desc)
		:TextureCube(_desc)
	{
		device = _device;

		imageFormat = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desc.format);

		vk::ImageCreateInfo imageInfo{};
		vma::AllocationCreateInfo allocInfo{};

		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = Cast<UInt32>(desc.width);
		imageInfo.extent.height = Cast<UInt32>(desc.height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = desc.mipLevels;
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
		barrier.subresourceRange.levelCount = desc.mipLevels;

		device->TransitionImageLayout(barrier);

		vk::ImageViewCreateInfo viewInfo{};
		viewInfo.image = textureImage.get();
		viewInfo.viewType = vk::ImageViewType::eCube;
		viewInfo.format = imageFormat;
		viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = desc.mipLevels;
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
}
