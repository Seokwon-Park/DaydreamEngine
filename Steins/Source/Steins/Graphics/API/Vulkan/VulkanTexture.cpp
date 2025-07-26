#include "SteinsPCH.h"
#include "VulkanTexture.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"
#include "Steins/Graphics/Utility/ImageLoader.h"
#include "backends/imgui_impl_vulkan.h"

namespace Steins
{
	VulkanTexture2D::VulkanTexture2D(VulkanRenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;
		desc = _desc;

		width = _desc.width;
		height = _desc.height;
		imageSize = width * height * 4;
		imageFormat = GraphicsUtil::RenderFormatToVkFormat(_desc.format);

		device->CreateImage(
			width,
			height,
			imageFormat,
			VK_IMAGE_TILING_OPTIMAL,
			GraphicsUtil::ConvertToVkImageUsageFlags(_desc.bindFlags),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			textureImage,
			textureImageMemory);

		device->TransitionTextureLayout(textureImage, imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		device->CreateImageView(textureImage, imageFormat, textureImageView);

		CreateSampler();
	}

	VulkanTexture2D::VulkanTexture2D(VulkanRenderDevice* _device, const FilePath& _path, const TextureDesc& _desc)
		:Texture2D(_path)
	{
		device = _device;

		Array<UInt8> imageData = ImageLoader::LoadImageFile(_path, width, height, channels);
		STEINS_CORE_ASSERT(!imageData.empty(), "Failed to load image!");
		imageSize = width * height * 4;

		device->CreateBuffer(
			imageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uploadBuffer,
			uploadBufferMemory);

		void* pixelData;
		vkMapMemory(device->GetDevice(), uploadBufferMemory, 0, imageSize, 0, &pixelData);
		memcpy(pixelData, imageData.data(), imageSize);
		vkUnmapMemory(device->GetDevice(), uploadBufferMemory);

		imageFormat = GraphicsUtil::RenderFormatToVkFormat(_desc.format);
		device->CreateImage(
			width,
			height,
			imageFormat,
			VK_IMAGE_TILING_OPTIMAL,
			GraphicsUtil::ConvertToVkImageUsageFlags(_desc.bindFlags),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			textureImage,
			textureImageMemory);

		device->TransitionTextureLayout(textureImage, imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		device->CopyBufferToImage(uploadBuffer, textureImage, width, height);
		device->TransitionTextureLayout(textureImage, imageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		device->CreateImageView(textureImage, imageFormat, textureImageView);

		vkDestroyBuffer(device->GetDevice(), uploadBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), uploadBufferMemory, nullptr);

		CreateSampler();

		ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(textureSampler, textureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	

	VulkanTexture2D::VulkanTexture2D(VulkanRenderDevice* _device, VkImage _image)
	{
		device = _device;
		textureImage = _image;
		isSwapchainImage = true;

		device->CreateImageView(textureImage, VK_FORMAT_R8G8B8A8_UNORM, textureImageView);

		CreateSampler();

		//ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(textureSampler, textureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanTexture2D::CreateSampler()
	{
		// sampler
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(device->GetGPU(), &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		//samplerInfo.anisotropyEnable = VK_FALSE;
		//samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		VkResult vr = vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &textureSampler);
		STEINS_CORE_ASSERT(vr == VK_SUCCESS, "failed to create texture sampler!");
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
		if (textureSampler != VK_NULL_HANDLE) vkDestroySampler(device->GetDevice(), textureSampler, nullptr);
		if (textureImageView != VK_NULL_HANDLE) vkDestroyImageView(device->GetDevice(), textureImageView, nullptr);
		if (!isSwapchainImage && textureImage != VK_NULL_HANDLE)
		{
			vkDestroyImage(device->GetDevice(), textureImage, nullptr);
		}
		if (!isSwapchainImage && textureImageMemory != VK_NULL_HANDLE) vkFreeMemory(device->GetDevice(), textureImageMemory, nullptr);
	}

	void* VulkanTexture2D::GetImGuiHandle()
	{
		if (ImGuiDescriptorSet != VK_NULL_HANDLE) return ImGuiDescriptorSet;
		return ImGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(textureSampler, textureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	VkImageView VulkanTexture2D::GetImageView()
	{
		return textureImageView;
	}

}
