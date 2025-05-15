#include "SteinsPCH.h"
#include "VulkanTexture.h"

#include "stb_image.h"
#include "backends/imgui_impl_vulkan.h"

namespace Steins
{
	VulkanTexture2D::VulkanTexture2D(VulkanGraphicsDevice* _device, const FilePath& _path)
		:Texture2D(_path)
	{
		device = _device;


		data = stbi_load(_path.ToString().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		STEINS_CORE_ASSERT(data, "Failed to load image!");
		imageSize = width * height * 4;
		device->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uploadBuffer, uploadBufferMemory);

		void* pixelData;
		vkMapMemory(device->GetDevice(), uploadBufferMemory, 0, imageSize, 0, &pixelData);
		memcpy(pixelData, data, imageSize);
		vkUnmapMemory(device->GetDevice(), uploadBufferMemory);

		stbi_image_free(data);

		device->CreateImage(width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

		device->TransitionTextureLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		device->CopyBufferToImage(uploadBuffer, textureImage, width, height);
		device->TransitionTextureLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		device->CreateImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, textureImageView);

		vkDestroyBuffer(device->GetDevice(), uploadBuffer, nullptr);
		vkFreeMemory(device->GetDevice(), uploadBufferMemory, nullptr);

		ImGui_ImplVulkan_AddTexture(textureSampler, textureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
		STEINS_CORE_ASSERT(vr = VK_SUCCESS, "failed to create texture sampler!");

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		{
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = device->GetDescriptorPool();
			allocInfo.descriptorSetCount = 1;
			//allocInfo.pSetLayouts = ;
			VkResult err = vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, &descriptorSet);
		}

		// Update the Descriptor Set:
		{
			VkDescriptorImageInfo imageDesc{};
			imageDesc.sampler = textureSampler;
			imageDesc.imageView = textureImageView;
			imageDesc.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			VkWriteDescriptorSet writeDesc{};
			writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDesc.dstSet = descriptorSet;
			writeDesc.descriptorCount = 1;
			writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeDesc.pImageInfo = &imageDesc;
			vkUpdateDescriptorSets(device->GetDevice(), 1, &writeDesc, 0, nullptr);
		}
	}
	VulkanTexture2D::~VulkanTexture2D()
	{
		vkDestroySampler(device->GetDevice(), textureSampler, nullptr);
		vkDestroyImageView(device->GetDevice(), textureImageView, nullptr);
		vkDestroyImage(device->GetDevice(), textureImage, nullptr);
		vkFreeMemory(device->GetDevice(), textureImageMemory, nullptr);
	}
}
