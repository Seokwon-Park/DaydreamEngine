#include "DaydreamPCH.h"
#include "VulkanPipelineState.h"

#include "VulkanSwapchain.h"
#include "Daydream/Graphics/Utility/GraphicsUtil.h"

#include "VulkanTexture.h"
#include "VulkanMaterial.h"

namespace Daydream
{
	VkDescriptorType ToVkDescType(ShaderResourceType _type)
	{
		switch (_type)
		{
		case ShaderResourceType::ConstantBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			break;
		case ShaderResourceType::Texture2D:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			break;
		default:
			break;
		}
		DAYDREAM_CORE_ASSERT(false, "Wrong type")
			return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}

	VulkanPipelineState::VulkanPipelineState(VulkanRenderDevice* _device, const PipelineStateDesc& _desc)
		:PipelineState(_desc)
	{
		device = _device;

		SortedMap<UInt32, Array<VkDescriptorSetLayoutBinding>> setBindings;

		VkVertexInputBindingDescription vertexInputdesc;
		vertexInputdesc.binding = 0;
		vertexInputdesc.stride = 0;
		vertexInputdesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		UInt32 offset = 0;
		
		Array<VkVertexInputAttributeDescription> attribDescArray;
		for (const Shared<Shader>& shader : shaders)
		{
			entryPoints[shader->GetType()] = GraphicsUtil::GetShaderEntryPointName(shader->GetType());
			

			VkPipelineShaderStageCreateInfo shaderStageInfo{};
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.module = (VkShaderModule)shader->GetNativeHandle();
			shaderStageInfo.pName = entryPoints[shader->GetType()].c_str();
			shaderStageInfo.stage = GraphicsUtil::GetVKShaderStage(shader->GetType());

			shaderStages.push_back(shaderStageInfo);
			for (const auto& info : shader->GetReflectionInfo())
			{
				if (info.shaderResourceType == ShaderResourceType::Input && shader->GetType() == ShaderType::Vertex)
				{
					VkVertexInputAttributeDescription attribDesc{};
					attribDesc.location = info.set;
					attribDesc.binding = info.binding;
					attribDesc.format = GraphicsUtil::ConvertRenderFormatToVkFormat(info.format);
					attribDesc.offset = offset;

					offset += info.size;
					vertexInputdesc.stride += info.size;

					attribDescArray.push_back(attribDesc);
					continue;
				}
				VkDescriptorSetLayoutBinding binding = {};
				binding.binding = info.binding;
				binding.descriptorType = ToVkDescType(info.shaderResourceType);
				binding.descriptorCount = 1;
				binding.stageFlags = GraphicsUtil::GetVKShaderStage(shader->GetType());
				binding.pImmutableSamplers = nullptr;
				setBindings[info.set].push_back(binding);
			}

		}

		for (const auto& [setIndex, bindings] : setBindings)
		{
			VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutCreateInfo.bindingCount = Cast<UInt32>(bindings.size());
			layoutCreateInfo.pBindings = bindings.data();

			VkDescriptorSetLayout layout;
			if (vkCreateDescriptorSetLayout(device->GetDevice(), &layoutCreateInfo, nullptr, &layout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}
			descriptorSetLayouts.push_back(layout);
		}

		Array<VkDynamicState> dynamicStates =
		{
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

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &vertexInputdesc; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<UInt32>(attribDescArray.size());
		vertexInputInfo.pVertexAttributeDescriptions = attribDescArray.data(); // Optional

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
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
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

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.pNext = nullptr; // 확장기능
		depthStencil.flags = 0; // 나중에 사용될 예약된 필드
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_TRUE;
		depthStencil.front.failOp = VK_STENCIL_OP_KEEP;           // 스텐실 테스트 실패 시
		depthStencil.front.passOp = VK_STENCIL_OP_KEEP;           // 스텐실 테스트 통과, 깊이 테스트 통과 시
		depthStencil.front.depthFailOp = VK_STENCIL_OP_KEEP;      // 스텐실 테스트 통과, 깊이 테스트 실패 시
		depthStencil.front.compareOp = VK_COMPARE_OP_ALWAYS;      // 스텐실 비교 연산
		depthStencil.front.compareMask = 0xFF;                    // 비교 마스크
		depthStencil.front.writeMask = 0xFF;                      // 쓰기 마스크
		depthStencil.front.reference = 0;                         // 참조 값

		// 일반적으로 front와 back은 동일하게 설정하거나, 양면 렌더링에 따라 다르게 설정합니다.
		depthStencil.back = depthStencil.front;
		//boundTest활성화 했을때만
		//depthStencil.minDepthBounds = 
		//depthStencil.maxDepthBounds = 


		//VkDescriptorPoolCreateInfo poolCreateInfo;
		//poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

		//VkDescriptorSetAllocateInfo allocInfo{};
		//allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		//allocInfo.descriptorPool = device->GetDescriptorPool();
		//allocInfo.descriptorSetCount = 1;
		//allocInfo.pSetLayouts = descriptorSetLayouts.data();

		//descriptorSets.resize(1);
		//if (vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		//	throw std::runtime_error("failed to allocate descriptor sets!");
		//}


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<UInt32>(descriptorSetLayouts.size()); // Optional
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data(); // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		Shared<VulkanRenderPass> rp = static_pointer_cast<VulkanRenderPass>(_desc.renderPass);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (UInt32)shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = rp->GetVkRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		VkResult result = vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
		DAYDREAM_CORE_ASSERT(result == VK_SUCCESS, "Failed to create pipeline!");

		//VkDescriptorBufferInfo bufferInfo{};
		//bufferInfo.buffer = (VkBuffer)_desc.constantBuffers[0].constantBuffer->GetNativeHandle();
		//bufferInfo.offset = 0;
		//bufferInfo.range = sizeof(Matrix4x4);

		//auto tex = (VulkanTexture2D*)_desc.textures[0].get();

		//VkDescriptorImageInfo imageInfo{};
		//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//imageInfo.imageView = tex->GetImageView();
		//imageInfo.sampler = tex->GetSampler();

		//descriptorWriteSets.resize(2);
		//descriptorWriteSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptorWriteSets[0].dstSet = descriptorSets[0];
		//descriptorWriteSets[0].dstBinding = 0;
		//descriptorWriteSets[0].dstArrayElement = 0;
		//descriptorWriteSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//descriptorWriteSets[0].descriptorCount = 1;
		//descriptorWriteSets[0].pBufferInfo = &bufferInfo;
		//descriptorWriteSets[0].pImageInfo = nullptr; // Optional
		//descriptorWriteSets[0].pTexelBufferView = nullptr; // Optional

		//descriptorWriteSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptorWriteSets[1].dstSet = descriptorSets[0];
		//descriptorWriteSets[1].dstBinding = 1;
		//descriptorWriteSets[1].dstArrayElement = 0;
		//descriptorWriteSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//descriptorWriteSets[1].descriptorCount = 1;
		//descriptorWriteSets[1].pBufferInfo = nullptr;
		//descriptorWriteSets[1].pImageInfo = &imageInfo; // Optional
		//descriptorWriteSets[1].pTexelBufferView = nullptr; // Optional

		//vkUpdateDescriptorSets(device->GetDevice(), (UInt32)descriptorWriteSets.size(), descriptorWriteSets.data(), 0, nullptr);
	}

	VulkanPipelineState::~VulkanPipelineState()
	{
		for (auto& layout : descriptorSetLayouts)
		{
			vkDestroyDescriptorSetLayout(device->GetDevice(), layout, nullptr);
		}
		vkDestroyPipeline(device->GetDevice(), pipeline, nullptr);
		vkDestroyPipelineLayout(device->GetDevice(), pipelineLayout, nullptr);
	}
	void VulkanPipelineState::Bind() const
	{
		//vkCmdBindDescriptorSets(device->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, descriptorSets.data(), 0, nullptr);
		vkCmdBindPipeline(device->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	void VulkanPipelineState::CreateShaderStageInfo(const Shared<Shader>& _shader)
	{

	}
	Shared<Material> VulkanPipelineState::CreateMaterial()
	{
		return MakeShared<VulkanMaterial>(device, this);
	}
}
