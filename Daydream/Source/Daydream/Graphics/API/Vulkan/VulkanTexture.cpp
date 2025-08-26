#include "DaydreamPCH.h"
#include "VulkanTexture.h"

#include "VulkanUtility.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "backends/imgui_impl_vulkan.h"

namespace Daydream
{
	VulkanTexture2D::VulkanTexture2D(VulkanRenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;
		desc = _desc;

		width = _desc.width;
		height = _desc.height;
		imageSize = width * height * 4;
		imageFormat = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desc.format);

		auto imageResource = device->CreateImage(width, height, imageFormat, vk::ImageTiling::eOptimal,
			GraphicsUtility::Vulkan::ConvertToVkImageUsageFlags(_desc.bindFlags), vk::MemoryPropertyFlagBits::eDeviceLocal);

		textureImage = vk::UniqueImage(imageResource.image, vk::detail::ObjectDestroy<vk::Device, vk::detail::DispatchLoaderDynamic>(device->GetDevice()));
		textureImageMemory = vk::UniqueDeviceMemory(imageResource.memory, vk::detail::ObjectFree<vk::Device, vk::detail::DispatchLoaderDynamic>(device->GetDevice()));

		device->TransitionImageLayout(textureImage.get(), imageFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		if (imageFormat == vk::Format::eD24UnormS8Uint)
		{
			auto imageView = device->CreateImageView(textureImage.get(), imageFormat,
				vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil);
			textureImageView = vk::UniqueImageView(imageView, vk::detail::ObjectDestroy<vk::Device, vk::detail::DispatchLoaderDynamic>(device->GetDevice()));
		}
		else
		{
			auto imageView = device->CreateImageView(textureImage.get(), imageFormat,
				vk::ImageAspectFlagBits::eColor);
			textureImageView = vk::UniqueImageView(imageView, vk::detail::ObjectDestroy<vk::Device, vk::detail::DispatchLoaderDynamic>(device->GetDevice()));
		}

		CreateSampler();
	}

	void VulkanTexture2D::CreateSampler()
	{
		vk::PhysicalDeviceProperties properties = device->GetPhysicalDevice().getProperties();

		vk::SamplerCreateInfo samplerInfo{};
		samplerInfo.magFilter = vk::Filter::eLinear;
		samplerInfo.minFilter = vk::Filter::eLinear;
		samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
		samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
		samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		//samplerInfo.anisotropyEnable = VK_FALSE;
		//samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = vk::CompareOp::eAlways;
		samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;

		textureSampler = device->GetDevice().createSamplerUnique(samplerInfo);
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
		ImGui_ImplVulkan_RemoveTexture(ImGuiDescriptorSet);
		ImGuiDescriptorSet = VK_NULL_HANDLE;
	}

	void* VulkanTexture2D::GetImGuiHandle()
	{
		if (ImGuiDescriptorSet != VK_NULL_HANDLE) return ImGuiDescriptorSet;
		return ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(textureSampler.get(), textureImageView.get(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanTexture2D::TransitionLayout(vk::ImageLayout _targetLayout)
	{
		if (currentLayout == _targetLayout) return;
		//device->TransitionTextureLayout(textureImage, imageFormat, currentLayout, _targetLayout);
		currentLayout = _targetLayout;
	}


}
