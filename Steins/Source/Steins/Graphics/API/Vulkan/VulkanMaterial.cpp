#include "SteinsPCH.h"
#include "VulkanMaterial.h"

namespace Steins
{
	VulkanMaterial::VulkanMaterial(VulkanRenderDevice* _device, VulkanPipelineState* _pso)
	{
		VkDescriptorPoolCreateInfo poolCreateInfo;
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

		const auto& layouts = _pso->GetLayout();

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _device->GetDescriptorPool();
		allocInfo.descriptorSetCount = layouts.size();
		allocInfo.pSetLayouts = layouts.data();

		
		
		VkResult result = vkAllocateDescriptorSets(_device->GetDevice(), &allocInfo, sets.data());
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to allocate descriptor sets!");
		for (auto shader : _pso->GetShaders())
		{
			auto tmp = shader->GetResourceInfo();
			for (auto info : tmp)
			{
				switch (info.type)
				{
				case ShaderResourceType::ConstantBuffer:
					break;
				case ShaderResourceType::Texture:
					break;
				}
			}
		}
	}
} 
