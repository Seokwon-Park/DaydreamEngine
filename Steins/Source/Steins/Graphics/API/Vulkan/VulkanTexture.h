#pragma once

#include "Steins/Graphics/Resources/Texture.h"
#include "VulkanRenderDevice.h"

namespace Steins
{
	class VulkanTexture2D :public Texture2D
	{
	public:
		VulkanTexture2D(VulkanRenderDevice* _device, const TextureDesc& _desc);
		VulkanTexture2D(VulkanRenderDevice* _device, const FilePath& _path, const TextureDesc& _desc);
		VulkanTexture2D(VulkanRenderDevice* _device, VkImage _image);
		void CreateSampler();
		virtual ~VulkanTexture2D();

		virtual void* GetNativeHandle() override { return textureImage; }
		virtual void* GetImGuiHandle() override;

		VkImage GetImage() { return textureImage; }
		VkImageView GetImageView();
		VkSampler GetSampler() { return textureSampler; }
	protected:

	private:
		bool isSwapchainImage = false;
		VulkanRenderDevice* device;
		VkDeviceSize imageSize;
		VkBuffer uploadBuffer;
		VkDeviceMemory uploadBufferMemory;

		VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;

		VkImage textureImage;
		VkImageView textureImageView = VK_NULL_HANDLE;
		VkDeviceMemory textureImageMemory;

		//ImGui Image¿ë
		VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;

		VkSampler textureSampler;
	};
}

