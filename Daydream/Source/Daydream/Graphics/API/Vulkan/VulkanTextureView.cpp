#include "DaydreamPCH.h"
#include "VulkanTextureView.h"
#include "Daydream/Graphics/Manager/SamplerRegistry.h"


namespace Daydream
{
	VulkanTextureView::VulkanTextureView(VulkanRenderDevice* _device, Shared<VulkanGPUTexture> _texture, const TextureViewDesc& _desc)
		:TextureView(_texture, _desc)
	{
		device = _device;

		vk::ImageViewCreateInfo viewInfo{};
		imageView = device->GetDevice().createImageViewUnique(viewInfo);

		if (_desc.type == TextureViewType::ShaderResource)
		{
			vk::DescriptorSetLayoutBinding samplerLayoutBinding{};
			samplerLayoutBinding.binding = 0;
			samplerLayoutBinding.descriptorCount = 1;
			samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			samplerLayoutBinding.pImmutableSamplers = nullptr;
			samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

			vk::DescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.bindingCount = 1;
			layoutInfo.pBindings = &samplerLayoutBinding;

			vk::UniqueDescriptorSetLayout myEngineImGuiLayout;
			myEngineImGuiLayout = device->GetDevice().createDescriptorSetLayoutUnique(layoutInfo);


			vk::DescriptorSetAllocateInfo allocInfo{};
			allocInfo.descriptorPool = device->GetDescriptorPool();
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &myEngineImGuiLayout.get();

			UISet = device->GetDevice().allocateDescriptorSetsUnique(allocInfo);

			Shared<VulkanSampler> sampler = SharedCast<VulkanSampler>(SamplerRegistry::LinearClampToEdge);

			vk::DescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageInfo.imageView = GetVkImageView();
			imageInfo.sampler = sampler->GetVkSampler();

			vk::WriteDescriptorSet descriptorWrite{};
			descriptorWrite.dstSet = UISet[0].get();
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;

			device->GetDevice().updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
		}

	}
}

