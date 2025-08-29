#include "DaydreamPCH.h"
#include "VulkanMaterial.h"
#include "VulkanTexture.h"
#include "VulkanTextureCube.h"
#include "VulkanBuffer.h"

namespace Daydream
{
	VulkanMaterial::VulkanMaterial(VulkanRenderDevice* _device, VulkanPipelineState* _pso)
	{
		device = _device;
		pso = _pso;
		vk::DescriptorPoolCreateInfo poolCreateInfo;

		const auto& layouts = _pso->GetLayout();

		vk::DescriptorSetAllocateInfo allocInfo{};
		allocInfo.descriptorPool = _device->GetDescriptorPool();
		allocInfo.descriptorSetCount = (UInt32)layouts.size();
		allocInfo.pSetLayouts = layouts.data();

		sets = _device->GetDevice().allocateDescriptorSetsUnique(allocInfo);
		DAYDREAM_CORE_INFO("Allocated {0} descriptor sets", sets.size());

		for (auto shader : _pso->GetShaders())
		{
			auto resources = shader->GetReflectionInfo();
			for (auto resource : resources)
			{
				bindingMap[resource.name] = resource;
			}
		}
	}
	void VulkanMaterial::Bind()
	{
		device->GetCommandBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pso->GetPipelineLayout(), 0, 1, &sets.data()->get(), 0, nullptr);
	}
	void VulkanMaterial::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			auto resourceInfo = bindingMap[_name];
			Shared<VulkanTexture2D> texture = static_pointer_cast<VulkanTexture2D>(_texture);
			vk::DescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageInfo.imageView = texture->GetImageView();
			imageInfo.sampler = texture->GetSampler();

			vk::WriteDescriptorSet writeSet = {};
			writeSet.dstSet = sets[resourceInfo.set].get();
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			writeSet.pImageInfo = &imageInfo;

			device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);
		}
	}

	void VulkanMaterial::SetTextureCube(const std::string& _name, Shared<TextureCube> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			auto resourceInfo = bindingMap[_name];
			Shared<VulkanTextureCube> texture = static_pointer_cast<VulkanTextureCube>(_texture);
			vk::DescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageInfo.imageView = texture->GetImageView();
			imageInfo.sampler = texture->GetSampler();

			vk::WriteDescriptorSet writeSet = {};
			writeSet.dstSet = sets[resourceInfo.set].get();
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			writeSet.pImageInfo = &imageInfo;

			device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);
		}
	}
	void VulkanMaterial::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			auto resourceInfo = bindingMap[_name];
			if (resourceInfo.shaderResourceType != ShaderResourceType::ConstantBuffer) return;

			Shared<VulkanConstantBuffer> buffer = static_pointer_cast<VulkanConstantBuffer>(_buffer);
			vk::DescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = (VkBuffer)_buffer->GetNativeHandle();
			bufferInfo.offset = 0;
			bufferInfo.range = buffer->GetSize();

			vk::WriteDescriptorSet writeSet = {};
			writeSet.dstSet = sets[resourceInfo.set].get();
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
			writeSet.pBufferInfo = &bufferInfo;

			device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);

		}
	}
}
