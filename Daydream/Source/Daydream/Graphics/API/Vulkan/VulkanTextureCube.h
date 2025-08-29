#pragma once

#include "Daydream/Graphics/Resources/TextureCube.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanTextureCube : public TextureCube
	{
	public:
		// Constrcuter Destructer
		VulkanTextureCube(VulkanRenderDevice* _device, const TextureDesc& _desc);
		virtual ~VulkanTextureCube();

		virtual inline void* GetNativeHandle() override { return textureImage.get(); }

		vk::Image GetImage() { return textureImage.get(); }
		vk::ImageView GetImageView() { return textureImageView.get(); }
		vk::Sampler GetSampler() { return textureSampler.get(); }

		void CreateSampler();

	private:
		VulkanRenderDevice* device;

		vk::DeviceSize imageSize;

		vk::Format imageFormat = vk::Format::eR8G8B8A8Srgb;

		vma::UniqueImage textureImage;
		vma::UniqueAllocation textureImageAllocation;
		vk::UniqueImageView textureImageView;

		vk::UniqueSampler textureSampler;
	};
}
