#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanTexture2D :public Texture2D
	{
	public:
		VulkanTexture2D(VulkanRenderDevice* _device, const TextureDesc& _desc); // Dynamic(RenderTarget, ShaderResource...)
		//VulkanTexture2D(VulkanRenderDevice* _device, const FilePath& _path, const TextureDesc& _desc); // FileLoad
		//VulkanTexture2D(VulkanRenderDevice* _device, VkImage _image, VkFormat _format); // ForSwapchain
		void CreateSampler();
		virtual ~VulkanTexture2D();

		virtual void* GetNativeHandle() override { return textureImage.get(); }
		virtual void* GetImGuiHandle() override;

		vk::Image GetImage() { return textureImage.get(); }
		vk::ImageView GetImageView() { return textureImageView.get(); }
		vk::Sampler GetSampler() { return textureSampler.get(); }
		vk::DescriptorSet GetDescriptorSet() { return textureSet[0].get(); }

		void AllocateDescriptorSet(vk::DescriptorSetLayout _layout);
	protected:

	private:
		bool isSwapchainImage = false;
		VulkanRenderDevice* device;
		vk::DeviceSize imageSize;

		vk::Format imageFormat = vk::Format::eR8G8B8A8Srgb;

		vma::UniqueImage textureImage;
		vma::UniqueAllocation textureImageAllocation;
		vk::UniqueImageView textureImageView;
		Array<vk::UniqueDescriptorSet> textureSet;
		Array<vk::UniqueDescriptorSet> oldTextureSet;

		//ImGui Image¿ë
		VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;

		vk::UniqueSampler textureSampler;
	};
}

