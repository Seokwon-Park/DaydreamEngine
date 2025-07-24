#pragma once

#include "Steins/Graphics/Resources/Material.h"
#include "Steins/Graphics/API/Vulkan/VulkanRenderDevice.h"
#include "Steins/Graphics/API/Vulkan/VulkanPipelineState.h"

namespace Steins
{
	class VulkanMaterial : public Material
	{
	public:
		VulkanMaterial(VulkanRenderDevice* _device, VulkanPipelineState* _pso);

		virtual void Bind() override;

		virtual void SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)override;
		virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)override;
	private:
		VulkanRenderDevice* device;
		VulkanPipelineState* pso;
		Array<VkDescriptorSet> sets;
	};
}