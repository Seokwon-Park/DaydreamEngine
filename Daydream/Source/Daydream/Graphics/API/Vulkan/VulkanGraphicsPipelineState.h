#pragma once

#include "Daydream/Graphics/Resources/PipelineState/GraphicsPipelineState.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanGraphicsPipelineState : public GraphicsPipelineState
	{
	public:
		VulkanGraphicsPipelineState(VulkanRenderDevice* _device, const GraphicsPipelineStateDesc& _desc);
		virtual ~VulkanGraphicsPipelineState() override;

		virtual void Bind() const override;
		//virtual Shared<Material> CreateMaterial() override;

		void CreateShaderStageInfo(const Shared<Shader>& _shader);
		Array<vk::DescriptorSetLayout> GetLayout() { return rawDescriptorSetLayouts; };
		vk::PipelineLayout GetPipelineLayout() { return pipelineLayout.get(); }
		vk::Pipeline GetPipeline() const { return pipeline.get(); }
	private:

		VulkanRenderDevice* device;
		Array<vk::PipelineShaderStageCreateInfo> shaderStages;

		vk::UniquePipeline pipeline;
		vk::UniquePipelineLayout pipelineLayout; // root signature
		Array<vk::UniqueDescriptorSetLayout> descriptorSetLayouts;
		Array<vk::DescriptorSetLayout> rawDescriptorSetLayouts;

		HashMap<ShaderType, String> entryPoints;

	};
}