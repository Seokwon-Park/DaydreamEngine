#pragma once

#include "Daydream/Graphics/Resources/Material.h"
#include "Daydream/Graphics/API/Vulkan/VulkanRenderDevice.h"
#include "Daydream/Graphics/API/Vulkan/VulkanPipelineState.h"

namespace Daydream
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(VulkanRenderDevice* _device, VulkanPipelineState* _pso);

		virtual void Bind() override;

		virtual void SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)override;
		virtual void SetTextureCube(const std::string& _name, Shared<TextureCube> _texture)override;
		virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)override;
	private:
		VulkanRenderDevice* device;
		VulkanPipelineState* pso;
		Array<vk::UniqueDescriptorSet> sets;
		Array<vk::DescriptorSet> rawSets;

		HashMap<String, Shared<ConstantBuffer>> cbuffers;
		HashMap<String, Shared<Texture2D>> textures;
		HashMap<String, Shared<TextureCube>> textureCubes; 

		vk::UniqueDescriptorSet descriptorSet;
	};
}