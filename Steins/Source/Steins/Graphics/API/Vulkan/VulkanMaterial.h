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

		virtual void Bind() override {};

	private:
		Array<VkDescriptorSet> sets;
		HashMap<String, VkWriteDescriptorSet> descriptorSets;
	};
}