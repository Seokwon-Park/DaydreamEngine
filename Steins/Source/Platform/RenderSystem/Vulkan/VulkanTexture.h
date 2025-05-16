#pragma once

#include "Steins/Render/Texture.h"
#include "VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanTexture2D :public Texture2D
	{
	public:
		VulkanTexture2D(VulkanGraphicsDevice* _device, const FilePath& _path);
		virtual ~VulkanTexture2D();

		virtual void* GetNativeHandle() override { return descriptorSet; }
		virtual void Bind(UInt32 _slot) const override {};

		VkImage GetImage() { return textureImage; }
		VkImageView GetImageView() { return textureImageView; }
		VkSampler GetSampler() { return textureSampler; }
	protected:

	private:
		VulkanGraphicsDevice* device;
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

