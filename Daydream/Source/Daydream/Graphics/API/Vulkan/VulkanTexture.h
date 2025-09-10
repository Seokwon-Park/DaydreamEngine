#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "VulkanRenderDevice.h"
#include "VulkanSampler.h"

namespace Daydream
{
	class VulkanTexture2D :public Texture2D
	{
	public:
		VulkanTexture2D(VulkanRenderDevice* _device, const TextureDesc& _desc); // Dynamic(RenderTarget, ShaderResource...)
		//VulkanTexture2D(VulkanRenderDevice* _device, const FilePath& _path, const TextureDesc& _desc); // FileLoad
		//VulkanTexture2D(VulkanRenderDevice* _device, VkImage _image, VkFormat _format); // ForSwapchain
		virtual ~VulkanTexture2D();

		virtual void SetSampler(Shared<Sampler> _sampler) override;

		virtual void* GetNativeHandle() override { return textureImage.get(); }
		virtual void* GetImGuiHandle() override;

		vk::Image GetImage() { return textureImage.get(); }
		vk::ImageView GetImageView() { return textureImageView.get(); }
		vk::Sampler GetSampler() { return textureSampler->GetSampler(); }

	protected:

	private:
		bool isSwapchainImage = false;
		VulkanRenderDevice* device;
		VulkanSampler* textureSampler;

		vma::UniqueImage textureImage;
		vma::UniqueAllocation textureImageAllocation;
		vk::UniqueImageView textureImageView;

		//ImGui Image¿ë
		VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;
	};
}

