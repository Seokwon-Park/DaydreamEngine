#include "SteinsPCH.h"
#include "VulkanPipelineState.h"

namespace Steins
{
	VulkanPipelineState::VulkanPipelineState(VulkanGraphicsDevice* _device, PipelineStateDesc _desc)
	{
		device = _device;
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};

		{
			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			//pipelineInfo.pVertexInputState = &vertexInputInfo;
			//pipelineInfo.pInputAssemblyState = &inputAssembly;
			//pipelineInfo.pViewportState = &viewportState;
			//pipelineInfo.pRasterizationState = &rasterizer;
			//pipelineInfo.pMultisampleState = &multisampling;
			//pipelineInfo.pDepthStencilState = nullptr; // Optional
			//pipelineInfo.pColorBlendState = &colorBlending;
			//pipelineInfo.pDynamicState = &dynamicState;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = device->GetRenderPass();
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
			pipelineInfo.basePipelineIndex = -1; // Optional

			VkResult result = vkCreateGraphicsPipelines(device->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create pipeline!");
		}

	}
	VulkanPipelineState::~VulkanPipelineState()
	{
		vkDestroyPipeline(device->GetLogicalDevice(), pipeline, nullptr);
		vkDestroyPipelineLayout(device->GetLogicalDevice(), pipelineLayout, nullptr);
		
	}
	void VulkanPipelineState::Bind() const
	{
	}
}
