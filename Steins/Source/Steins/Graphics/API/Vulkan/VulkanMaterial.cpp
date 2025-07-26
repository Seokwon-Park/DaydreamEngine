#include "SteinsPCH.h"
#include "VulkanMaterial.h"
#include "VulkanTexture.h"
#include "VulkanBuffer.h"

namespace Steins
{
	VulkanMaterial::VulkanMaterial(VulkanRenderDevice* _device, VulkanPipelineState* _pso)
	{
		device = _device;
		pso = _pso;
		VkDescriptorPoolCreateInfo poolCreateInfo;
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

		const auto& layouts = _pso->GetLayout();

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _device->GetDescriptorPool();
		allocInfo.descriptorSetCount = (UInt32)layouts.size();
		allocInfo.pSetLayouts = layouts.data();

		sets.resize(layouts.size());
		VkResult result = vkAllocateDescriptorSets(_device->GetDevice(), &allocInfo, sets.data());
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to allocate descriptor sets!");
		STEINS_CORE_INFO("Allocated {0} descriptor sets", sets.size());

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
		vkCmdBindDescriptorSets(device->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pso->GetPipelineLayout(), 0, 1, sets.data(), 0, nullptr);
	}
	void VulkanMaterial::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			auto resourceInfo = bindingMap[_name];
			Shared<VulkanTexture2D> texture = static_pointer_cast<VulkanTexture2D>(_texture);
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = texture->GetImageView();
			imageInfo.sampler = texture->GetSampler();

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet = sets[resourceInfo.set];
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeSet.pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device->GetDevice(), 1, &writeSet, 0, nullptr);
		}
	}
	void VulkanMaterial::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			auto resourceInfo = bindingMap[_name];
			if (resourceInfo.shaderResourceType != ShaderResourceType::ConstantBuffer) return;

			Shared<VulkanConstantBuffer> buffer = static_pointer_cast<VulkanConstantBuffer>(_buffer);
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = (VkBuffer)_buffer->GetNativeHandle();
			bufferInfo.offset = 0;
			bufferInfo.range = buffer->GetSize();

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet = sets[resourceInfo.set];
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeSet.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(device->GetDevice(), 1, &writeSet, 0, nullptr);
		}
	}
}
