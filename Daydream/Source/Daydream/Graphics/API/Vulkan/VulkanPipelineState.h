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
		Array<vk::DescriptorSetLayout> GetLayout() { return rawDescriptorSetLayouts; };
		vk::PipelineLayout GetPipelineLayout() { return pipelineLayout.get(); }
	private:

		VulkanRenderDevice* device;
		Array<vk::PipelineShaderStageCreateInfo> shaderStages;

		vk::UniquePipeline pipeline;
		vk::UniquePipelineLayout pipelineLayout; // root signature
		Array<vk::UniqueDescriptorSetLayout> descriptorSetLayouts;
		Array<vk::DescriptorSetLayout> rawDescriptorSetLayouts;

		vk::DescriptorPool descriptorPool;

		HashMap<ShaderType, String> entryPoints;

	};
}