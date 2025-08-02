#pragma once

#include "Daydream/Graphics/Core/PipelineState.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanPipelineState : public PipelineState
	{
	public:
		VulkanPipelineState(VulkanRenderDevice* _device, const PipelineStateDesc& _desc);
		virtual ~VulkanPipelineState() override;

		virtual void Bind() const override;
		virtual Shared<Material> CreateMaterial() override;

		void CreateShaderStageInfo(const Shared<Shader>& _shader);
		Array<VkDescriptorSetLayout> GetLayout() { return descriptorSetLayouts; };
		VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
	private:

		VulkanRenderDevice* device;
		Array<VkPipelineShaderStageCreateInfo> shaderStages;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout; // root signature
		Array<VkDescriptorSetLayout> descriptorSetLayouts;

		VkDescriptorPool descriptorPool;
		Array<VkDescriptorSet> descriptorSets;
		Array<VkWriteDescriptorSet> descriptorWriteSets;

		HashMap<ShaderType, String> entryPoints;

	};
}