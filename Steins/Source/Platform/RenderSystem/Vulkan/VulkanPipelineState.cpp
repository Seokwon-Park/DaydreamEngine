#include "SteinsPCH.h"
#include "VulkanPipelineState.h"

#include "VulkanSwapChain.h"
#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	VulkanPipelineState::VulkanPipelineState(VulkanGraphicsDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;
		
		for (Shared<Shader> shader : shaders)
		{
			CreateShaderStageInfo(shader);
		}

		{
			std::vector<VkDynamicState> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
			};

			VkPipelineDynamicStateCreateInfo dynamicState{};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			dynamicState.pDynamicStates = dynamicStates.data();

			VkPipelineViewportStateCreateInfo viewportState{};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.scissorCount = 1;

			VkVertexInputBindingDescription desc;
			desc.binding = 0;
			desc.stride = 28;
			desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			VkVertexInputAttributeDescription attribdesc[2];
			attribdesc[0].location = 0;
			attribdesc[0].binding = 0;
			attribdesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attribdesc[0].offset = 0;

			attribdesc[1].location = 1;
			attribdesc[1].binding = 0;
			attribdesc[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			attribdesc[1].offset = 12;

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &desc; // Optional
			vertexInputInfo.vertexAttributeDescriptionCount = 2;
			vertexInputInfo.pVertexAttributeDescriptions = attribdesc; // Optional

			VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkPipelineRasterizationStateCreateInfo rasterizer{};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f; // Optional
			rasterizer.depthBiasClamp = 0.0f; // Optional
			rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

			VkPipelineMultisampleStateCreateInfo multisampling{};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampling.minSampleShading = 1.0f; // Optional
			multisampling.pSampleMask = nullptr; // Optional
			multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
			multisampling.alphaToOneEnable = VK_FALSE; // Optional

			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

			VkPipelineColorBlendStateCreateInfo colorBlending{};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f; // Optional
			colorBlending.blendConstants[1] = 0.0f; // Optional
			colorBlending.blendConstants[2] = 0.0f; // Optional
			colorBlending.blendConstants[3] = 0.0f; // Optional

			/*		VkDescriptorSetLayoutBinding uboLayoutBinding{};
					uboLayoutBinding.binding = 0;
					uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					uboLayoutBinding.descriptorCount = 1;
					uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

					VkDescriptorSetLayoutCreateInfo layoutInfo{};
					layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
					layoutInfo.bindingCount = 1;
					layoutInfo.pBindings = &uboLayoutBinding;

					if (vkCreateDescriptorSetLayout(device->GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
						throw std::runtime_error("failed to create descriptor set layout!");
					}*/

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0; // Optional
			pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
			pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
			pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

			if (vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create pipeline layout!");
			}

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = shaderStages.size();
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pDepthStencilState = nullptr; // Optional
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.pDynamicState = &dynamicState;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = device->GetMainRenderPass();
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
			pipelineInfo.basePipelineIndex = -1; // Optional

			VkResult result = vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create pipeline!");
		}
		//		VkBuffer uniformBuffer;
		//VkDeviceMemory uniformBufferMemory;

		//VkBufferCreateInfo bufferInfo{};
		//bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//bufferInfo.size = 64;
		//bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		//bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//VkResult result = vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &uniformBuffer);
		//STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create vertex buffer!");

		//VkMemoryRequirements memRequirements;
		//vkGetBufferMemoryRequirements(device->GetDevice(), uniformBuffer, &memRequirements);

		//VkMemoryAllocateInfo allocInfo{};
		//allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//allocInfo.allocationSize = memRequirements.size;
		//allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits,
		//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		//if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &uniformBufferMemory) != VK_SUCCESS)
		//{
		//	throw std::runtime_error("failed to allocate vertex buffer memory!");
		//}

		//Matrix4x4 mat{};

		//vkBindBufferMemory(device->GetDevice(), uniformBuffer, uniformBufferMemory, 0);
		//void* data;
		//vkMapMemory(device->GetDevice(), uniformBufferMemory, 0, bufferInfo.size, 0, &data);
		//memcpy(data, &mat.glmMatrix, (size_t)bufferInfo.size);
		//vkUnmapMemory(device->GetDevice(), uniformBufferMemory);



	}


	VulkanPipelineState::~VulkanPipelineState()
	{
		//vkDestroyDescriptorSetLayout(device->GetDevice(), descriptorSetLayout, nullptr);
		vkDestroyPipeline(device->GetDevice(), pipeline, nullptr);
		vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, nullptr);
	}
	void VulkanPipelineState::Bind() const
	{
		vkCmdBindPipeline(device->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		VkDeviceSize bufferSize = sizeof(glm::mat4);

		//memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)1280;
		viewport.height = (float)720;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(device->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ 1280,720 });
		vkCmdSetScissor(device->GetCommandBuffer(), 0, 1, &scissor);
	}

	void VulkanPipelineState::CreateShaderStageInfo(const Shared<Shader>& _shader)
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.module = (VkShaderModule)_shader->GetNativeHandle();
		shaderStageInfo.pName = "main";
		shaderStageInfo.stage = GraphicsUtil::vulkanShaderStageMap[_shader->GetType()];

		shaderStages.push_back(shaderStageInfo);
	}
}
