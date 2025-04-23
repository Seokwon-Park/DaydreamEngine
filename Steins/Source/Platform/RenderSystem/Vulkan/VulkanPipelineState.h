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
	private:
		VulkanGraphicsDevice* device;
		VkPipelineShaderStageCreateInfo shaderStages[5];

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout; // root signature
	};
}