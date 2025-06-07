#pragma once

#include "Steins/Render/PipelineState.h"
#include "VulkanRenderDevice.h"

namespace Steins
{
	class VulkanPipelineState : public PipelineState
	{
	public:
		VulkanPipelineState(VulkanRenderDevice* _device, PipelineStateDesc _desc);
		virtual ~VulkanPipelineState() override;

		virtual void Bind() const override;

		void CreateShaderStageInfo(const Shared<Shader>& _shader);
	private:

		VulkanRenderDevice* device;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout; // root signature
		VkDescriptorSetLayout descriptorSetLayout;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkWriteDescriptorSet> descriptorWriteSets;

	};
}