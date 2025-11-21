#include "DaydreamPCH.h"
#include "VulkanTexture.h"

#include "VulkanUtility.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "backends/imgui_impl_vulkan.h"

namespace Daydream
{
	VulkanTexture2D::VulkanTexture2D(VulkanRenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;
		desc = _desc;

		width = _desc.width;
		height = _desc.height;
		vk::Format imageFormat = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desc.format);

		vk::ImageCreateInfo imageInfo{};
		vma::AllocationCreateInfo allocInfo{};

		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = static_cast<UInt32>(width);
		imageInfo.extent.height = static_cast<UInt32>(height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = imageFormat;
		imageInfo.tiling = vk::ImageTiling::eOptimal;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		imageInfo.usage = GraphicsUtility::Vulkan::ConvertToVkImageUsageFlags(_desc.bindFlags);
		imageInfo.sharingMode = vk::SharingMode::eExclusive;
		imageInfo.samples = vk::SampleCountFlagBits::e1;

		allocInfo.usage = vma::MemoryUsage::eGpuOnly;
		allocInfo.flags = {};

		std::tie(textureImage, textureImageAllocation) = device->CreateImage(imageInfo, allocInfo);

		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = vk::ImageLayout::eUndefined;
		barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = textureImage.get();
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		if (imageFormat == vk::Format::eD24UnormS8Uint)
		{
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;

			device->TransitionImageLayout(barrier);

			textureImageView = device->CreateImageView(textureImage.get(), imageFormat,
				vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil);
		}
		else
		{
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

			device->TransitionImageLayout(barrier);

			textureImageView = device->CreateImageView(textureImage.get(), imageFormat,
				vk::ImageAspectFlagBits::eColor);
		}
	}


	VulkanTexture2D::~VulkanTexture2D()
	{
		if (ImGuiDescriptorSet != VK_NULL_HANDLE)
		{
			ImGui_ImplVulkan_RemoveTexture(ImGuiDescriptorSet);
			ImGuiDescriptorSet = VK_NULL_HANDLE;
		}
	}

	void VulkanTexture2D::SetSampler(Shared<Sampler> _sampler)
	{
		textureSampler = static_cast<VulkanSampler*>(_sampler.get());
	}

	void* VulkanTexture2D::GetImGuiHandle()
	{
		if (ImGuiDescriptorSet != VK_NULL_HANDLE) return ImGuiDescriptorSet;
		auto imguiSampler = static_cast<VulkanSampler*>(ResourceManager::GetResource<Sampler>("LinearRepeat").get());
		return ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(imguiSampler->GetSampler(), textureImageView.get(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	//void VulkanTexture2D::AllocateDescriptorSet(vk::DescriptorSetLayout _layout)
	//{
	//	vk::DescriptorSetAllocateInfo allocInfo{};
	//	allocInfo.descriptorPool = device->GetDescriptorPool();
	//	allocInfo.descriptorSetCount = 1;
	//	allocInfo.pSetLayouts = &_layout;

	//	textureSet = device->GetDevice().allocateDescriptorSetsUnique(allocInfo);
	//}

}
