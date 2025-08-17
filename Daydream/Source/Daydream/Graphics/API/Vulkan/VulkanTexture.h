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

		void TransitionLayout(vk::ImageLayout _targetLayout);

	protected:

	private:
		bool isSwapchainImage = false;
		VulkanRenderDevice* device;
		vk::DeviceSize imageSize;
		vk::Buffer uploadBuffer;
		vk::DeviceMemory uploadBufferMemory;

		vk::Format imageFormat = vk::Format::eR8G8B8A8Srgb;

		vk::UniqueImage textureImage;
		vk::UniqueImageView textureImageView;
		vk::UniqueDeviceMemory textureImageMemory;

		//ImGui Image¿ë
		VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;

		vk::UniqueSampler textureSampler;

		vk::ImageLayout currentLayout = vk::ImageLayout::eUndefined;
	};
}

