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
		
		virtual inline void* GetNativeHandle() override { return textureImage.get(); }

		vk::Image GetImage() { return textureImage.get(); }
		vk::ImageView GetImageView() { return textureImageView.get(); }
		vk::Sampler GetSampler() { return textureSampler->GetSampler(); }
	private:
		VulkanRenderDevice* device;
		VulkanSampler* textureSampler;

		vma::UniqueImage textureImage;
		vma::UniqueAllocation textureImageAllocation;
		vk::UniqueImageView textureImageView;


	};
}
