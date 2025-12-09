#pragma once

#include "Daydream/Graphics/Resources/TextureCube.h"
#include "VulkanRenderDevice.h"
#include "VulkanSampler.h"

namespace Daydream
{
	class VulkanTextureCube : public TextureCube
	{
	public:
		// Constrcuter Destructer
		VulkanTextureCube(VulkanRenderDevice* _device, const TextureDesc& _desc);
		virtual ~VulkanTextureCube();

		virtual void SetSampler(Shared<Sampler> _sampler) override;
		virtual bool HasSampler() override { return textureSampler != nullptr; }

		virtual void GenerateMips() override;
		
		virtual inline void* GetNativeHandle() override { return textureImage.get(); }

		UInt32 GetMipLevels() { return mipLevels; }

		vk::Image GetImage() { return textureImage.get(); }
		vk::ImageView GetImageView() { return textureImageView.get(); }
		vk::Sampler GetSampler() { return textureSampler->GetSampler(); }
	private:
		VulkanRenderDevice* device;
		VulkanSampler* textureSampler;

		vma::UniqueImage textureImage;
		vma::UniqueAllocation textureImageAllocation;
		vk::UniqueImageView textureImageView;

		vk::Format imageFormat;
		UInt32 mipLevels = 0;

	};
}
