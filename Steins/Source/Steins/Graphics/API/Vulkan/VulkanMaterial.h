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

		virtual void Bind() override {
			vkCmdBindDescriptorSets(device->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pso->GetPipelineLayout(), 0, 1, sets.data(), 0, nullptr);
		};
		
		void SetTexture(const std::string& _name, Shared<Texture> _texture);
		void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer);


	private:
		VulkanRenderDevice* device;
		VulkanPipelineState* pso;
		Array<VkDescriptorSet> sets;
		HashMap<String, ShaderResource> bindingMap;
	};
}