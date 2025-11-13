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

		for (auto& set : sets)
		{
			rawSets.push_back(set.get());
		}

		auto resources = pso->GetShaderGroup()->GetShaderResourceData();
		for (auto resource : resources)
		{
			bindingMap[resource.name] = resource;
		}
	}
	VulkanMaterial::~VulkanMaterial()
	{
		device = nullptr;
		pso = nullptr;
		rawSets.clear();
		sets.clear();
	}
	void VulkanMaterial::Bind()
	{
		for (auto [name, texture] : textures)
		{
			if (texture == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			Shared<VulkanTexture2D> vulkanTexture = static_pointer_cast<VulkanTexture2D>(texture);

			vk::DescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageInfo.imageView = vulkanTexture->GetImageView();
			imageInfo.sampler = vulkanTexture->GetSampler();

			vk::WriteDescriptorSet writeSet = {};
			writeSet.dstSet = sets[resourceInfo.set].get();
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			writeSet.pImageInfo = &imageInfo;

			device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);
		}

		for (auto [name, texture] : textureCubes)
		{
			if (texture == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			Shared<VulkanTextureCube> vulkanTexture = static_pointer_cast<VulkanTextureCube>(texture);

			vk::DescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageInfo.imageView = vulkanTexture->GetImageView();
			imageInfo.sampler = vulkanTexture->GetSampler();

			vk::WriteDescriptorSet writeSet = {};
			writeSet.dstSet = sets[resourceInfo.set].get();
			writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			writeSet.pImageInfo = &imageInfo;

			device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);
		}

		for (auto [name, cbuffer] : cbuffers)
		{
			if (cbuffer == nullptr) continue;
			auto resourceInfo = bindingMap[name];
			if (resourceInfo.shaderResourceType != ShaderResourceType::ConstantBuffer) return;

			Shared<VulkanConstantBuffer> buffer = static_pointer_cast<VulkanConstantBuffer>(cbuffer);
			vk::DescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = (VkBuffer)cbuffer->GetNativeHandle();
			bufferInfo.offset = 0;
			bufferInfo.range = buffer->GetSize();

			vk::WriteDescriptorSet writeSet = {};
			writeSet.dstSet = sets[resourceInfo.set].get();
			writeSet.dstBinding = resourceInfo.binding;
			writeSet.descriptorCount = 1;
			writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
			writeSet.pBufferInfo = &bufferInfo;

			device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);

		}

		device->GetCommandBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pso->GetPipelineLayout(), 0, (UInt32)rawSets.size(), rawSets.data(), 0, nullptr);
	}
	//void VulkanMaterial::SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)
	//{
	//	if (_texture == nullptr) return;
	//	//if (bindingMap.find(_name) != bindingMap.end())
	//	//{
	//	//	auto resourceInfo = bindingMap[_name];
	//	//	Shared<VulkanTexture2D> texture = static_pointer_cast<VulkanTexture2D>(_texture);

	//	//	vk::DescriptorImageInfo imageInfo{};
	//	//	imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	//	//	imageInfo.imageView = texture->GetImageView();
	//	//	imageInfo.sampler = texture->GetSampler();

	//	//	vk::WriteDescriptorSet writeSet = {};
	//	//	writeSet.dstSet = sets[resourceInfo.set].get();
	//	//	writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
	//	//	writeSet.descriptorCount = 1;
	//	//	writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	//	//	writeSet.pImageInfo = &imageInfo;

	//	//	device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);

	//	//	//rawSets[resourceInfo.set] = texture->GetDescriptorSet();
	//	//}

	//	if (bindingMap.find(_name) != bindingMap.end())//&& textures.find(_name) != textures.end())
	//	{
	//		auto resourceInfo = bindingMap[_name];
	//		textures[_name] = _texture;
	//	}
	//}

	//void VulkanMaterial::SetTextureCube(const std::string& _name, Shared<TextureCube> _texture)
	//{
	//	if (_texture == nullptr) return;
	//	//if (bindingMap.find(_name) != bindingMap.end())
	//	//{
	//	//	auto resourceInfo = bindingMap[_name];
	//	//	Shared<VulkanTextureCube> texture = static_pointer_cast<VulkanTextureCube>(_texture);
	//	//	vk::DescriptorImageInfo imageInfo{};
	//	//	imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	//	//	imageInfo.imageView = texture->GetImageView();
	//	//	imageInfo.sampler = texture->GetSampler();

	//	//	vk::WriteDescriptorSet writeSet = {};
	//	//	writeSet.dstSet = sets[resourceInfo.set].get();
	//	//	writeSet.dstBinding = resourceInfo.binding;  // 특정 binding만 업데이트
	//	//	writeSet.descriptorCount = 1;
	//	//	writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	//	//	writeSet.pImageInfo = &imageInfo;

	//	//	device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);
	//	//}

	//	if (bindingMap.find(_name) != bindingMap.end())//&& textures.find(_name) != textures.end())
	//	{
	//		auto resourceInfo = bindingMap[_name];
	//		textureCubes[_name] = _texture;
	//	}
	//}
	//void VulkanMaterial::SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)
	//{
	//	//if (bindingMap.find(_name) != bindingMap.end())
	//	//{
	//	//	auto resourceInfo = bindingMap[_name];
	//	//	if (resourceInfo.shaderResourceType != ShaderResourceType::ConstantBuffer) return;

	//	//	Shared<VulkanConstantBuffer> buffer = static_pointer_cast<VulkanConstantBuffer>(_buffer);
	//	//	vk::DescriptorBufferInfo bufferInfo{};
	//	//	bufferInfo.buffer = (VkBuffer)_buffer->GetNativeHandle();
	//	//	bufferInfo.offset = 0;
	//	//	bufferInfo.range = buffer->GetSize();

	//	//	vk::WriteDescriptorSet writeSet = {};
	//	//	writeSet.dstSet = sets[resourceInfo.set].get();
	//	//	writeSet.dstBinding = resourceInfo.binding;
	//	//	writeSet.descriptorCount = 1;
	//	//	writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
	//	//	writeSet.pBufferInfo = &bufferInfo;

	//	//	device->GetDevice().updateDescriptorSets(1, &writeSet, 0, nullptr);

	//	//}

	//	if (bindingMap.find(_name) != bindingMap.end())//&& textures.find(_name) != textures.end())
	//	{
	//		auto resourceInfo = bindingMap[_name];
	//		cbuffers[_name] = _buffer;
	//	}
	//}
}
