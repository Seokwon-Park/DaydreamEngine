#pragma once

#include "Steins/Graphics/Resources/Texture.h"
#include "VulkanRenderDevice.h"

namespace Steins
{
	class VulkanTexture2D :public Texture2D
	{
	public:
		VulkanTexture2D(VulkanRenderDevice* _device, const FilePath& _path);
		virtual ~VulkanTexture2D();

		virtual void* GetNativeHandle() override { return descriptorSet; }
		virtual void Bind(UInt32 _slot) const override {};

		VkImage GetImage() { return textureImage; }
		VkImageView GetImageView() { return textureImageView; }
		VkSampler GetSampler() { return textureSampler; }
	protected:

	private:
		VulkanRenderDevice* device;
		VkDeviceSize imageSize;
		VkBuffer uploadBuffer;
		VkDeviceMemory uploadBufferMemory;

		VkImage textureImage;
		VkImageView textureImageView;
		VkDeviceMemory textureImageMemory;
		VkDescriptorSet descriptorSet;


		VkSampler textureSampler;
	};
}

