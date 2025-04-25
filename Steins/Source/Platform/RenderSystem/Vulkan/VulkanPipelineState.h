#pragma once

#include "Steins/Render/PipelineState.h"
#include "VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanPipelineState : public PipelineState
	{
	public:
		VulkanPipelineState(VulkanGraphicsDevice* _device, PipelineStateDesc _desc);
		virtual ~VulkanPipelineState() override;

		virtual void Bind() const override;

		void CreateShaderStageInfo(std::vector<Shared<Shader>>& _shaders);
	private:
		VulkanGraphicsDevice* device;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout; // root signature
		VkDescriptorSetLayout descriptorSetLayout;

	};
}