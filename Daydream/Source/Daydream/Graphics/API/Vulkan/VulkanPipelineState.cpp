#include "DaydreamPCH.h"
#include "VulkanPipelineState.h"

#include "VulkanSwapchain.h"
#include "VulkanUtility.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

#include "VulkanTexture.h"
#include "VulkanMaterial.h"
#include "VulkanShader.h"
#include "VulkanRenderPass.h"

namespace Daydream
{
	vk::DescriptorType ToVkDescType(ShaderResourceType _type)
	{
		switch (_type)
		{
		case ShaderResourceType::ConstantBuffer:
			return vk::DescriptorType::eUniformBuffer;
			break;
		case ShaderResourceType::Texture:
			return vk::DescriptorType::eCombinedImageSampler;
			break;
		default:
			break;
		}
		DAYDREAM_CORE_ASSERT(false, "Wrong type")
			return {};
	}

	VulkanPipelineState::VulkanPipelineState(VulkanRenderDevice* _device, const PipelineStateDesc& _desc)
		:PipelineState(_desc)
	{
		device = _device;

		SortedMap<UInt32, Array<vk::DescriptorSetLayoutBinding>> setBindings;

		vk::VertexInputBindingDescription vertexInputdesc;
		vertexInputdesc.binding = 0;
		vertexInputdesc.stride = 0;
		vertexInputdesc.inputRate = vk::VertexInputRate::eVertex;

		UInt32 offset = 0;

		Array<vk::VertexInputAttributeDescription> attribDescArray;
		for (const Shared<Shader>& shader : shaderGroup->GetShaders())
		{
			entryPoints[shader->GetType()] = GraphicsUtility::GetShaderEntryPointName(shader->GetType());

			Shared<VulkanShader> vulkanShader = static_pointer_cast<VulkanShader>(shader);

			vk::PipelineShaderStageCreateInfo shaderStageInfo{};
			shaderStageInfo.module = vulkanShader->GetShaderHandle();
			shaderStageInfo.pName = entryPoints[shader->GetType()].c_str();
			shaderStageInfo.stage = GraphicsUtility::Vulkan::ConvertToShaderStageFlagBit(shader->GetType());

			shaderStages.push_back(shaderStageInfo);
			UInt32 setSize = 0;
			for (const auto& info : shader->GetShaderReflectionData())
			{
				if (info.shaderResourceType == ShaderResourceType::Input && shader->GetType() == ShaderType::Vertex)
				{
					vk::VertexInputAttributeDescription attribDesc{};
					attribDesc.location = info.set;
					attribDesc.binding = info.binding;
					attribDesc.format = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(info.format);
					attribDesc.offset = offset;

					offset += info.size;
					vertexInputdesc.stride += info.size;

					attribDescArray.push_back(attribDesc);
					continue;
				}
				vk::DescriptorSetLayoutBinding binding = {};
				binding.binding = info.binding;
				binding.descriptorType = ToVkDescType(info.shaderResourceType);
				binding.descriptorCount = 1;
				binding.stageFlags = GraphicsUtility::Vulkan::ConvertToShaderStageFlagBit(shader->GetType());
				binding.pImmutableSamplers = nullptr;
				setBindings[info.set].push_back(binding);
			}
		}

		for (const auto& [setIndex, bindings] : setBindings)
		{
			Array<vk::DescriptorBindingFlags> bindingFlags(bindings.size(), vk::DescriptorBindingFlagBits::eUpdateAfterBind);

			vk::DescriptorSetLayoutBindingFlagsCreateInfo extendedInfo{};
			extendedInfo.bindingCount = bindingFlags.size();
			extendedInfo.pBindingFlags = bindingFlags.data();

			vk::DescriptorSetLayoutCreateInfo layoutCreateInfo{};
			layoutCreateInfo.pNext = &extendedInfo; // Ȯ�� ���� ����!
			layoutCreateInfo.bindingCount = Cast<UInt32>(bindings.size());
			layoutCreateInfo.pBindings = bindings.data();
			layoutCreateInfo.flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool;

			descriptorSetLayouts.push_back(device->GetDevice().createDescriptorSetLayoutUnique(layoutCreateInfo));
		}

		Array<vk::DynamicState> dynamicStates =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor,
		};

		vk::PipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.dynamicStateCount = static_cast<UInt32>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		vk::PipelineViewportStateCreateInfo viewportState{};
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &vertexInputdesc; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<UInt32>(attribDescArray.size());
		vertexInputInfo.pVertexAttributeDescriptions = attribDescArray.data(); // Optional

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		vk::PipelineRasterizationStateCreateInfo rasterizer = GraphicsUtility::Vulkan::TranslateToVulkanRasterizerCreateInfo(_desc.rasterizerState);
		//vk::PipelineRasterizationStateCreateInfo rasterizer{};
		//rasterizer.depthClampEnable = VK_FALSE;
		//rasterizer.rasterizerDiscardEnable = VK_FALSE;
		//rasterizer.polygonMode = vk::PolygonMode::eFill;
		//rasterizer.lineWidth = 1.0f;
		//rasterizer.cullMode = vk::CullModeFlagBits::eBack;
		//rasterizer.frontFace = vk::FrontFace::eClockwise;
		//rasterizer.depthBiasEnable = VK_FALSE;
		//rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		//rasterizer.depthBiasClamp = 0.0f; // Optional
		//rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		vk::PipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR
			| vk::ColorComponentFlagBits::eG
			| vk::ColorComponentFlagBits::eB
			| vk::ColorComponentFlagBits::eA;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne; // Optional
		colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero; // Optional
		colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero; // Optional
		colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd; // Optional

		vk::PipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = vk::LogicOp::eCopy; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		vk::PipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.pNext = nullptr; // Ȯ����
		depthStencil.flags = {}; // ���߿� ���� ����� �ʵ�
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = vk::CompareOp::eLess;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_TRUE;
		depthStencil.front.failOp = vk::StencilOp::eKeep;           // ���ٽ� �׽�Ʈ ���� ��
		depthStencil.front.passOp = vk::StencilOp::eKeep;           // ���ٽ� �׽�Ʈ ���, ���� �׽�Ʈ ��� ��
		depthStencil.front.depthFailOp = vk::StencilOp::eKeep;      // ���ٽ� �׽�Ʈ ���, ���� �׽�Ʈ ���� ��
		depthStencil.front.compareOp = vk::CompareOp::eAlways;      // ���ٽ� �� ����
		depthStencil.front.compareMask = 0xFF;                    // �� ����ũ
		depthStencil.front.writeMask = 0xFF;                      // ���� ����ũ
		depthStencil.front.reference = 0;                         // ���� ��

		// �Ϲ������� front�� back�� �����ϰ� �����ϰų�, ��� �������� ���� �ٸ��� �����մϴ�.
		depthStencil.back = depthStencil.front;

		rawDescriptorSetLayouts.clear();
		for (const auto& uniqueLayout : descriptorSetLayouts)
		{
			rawDescriptorSetLayouts.push_back(uniqueLayout.get());
		}

		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.setLayoutCount = static_cast<UInt32>(rawDescriptorSetLayouts.size()); // Optional
		pipelineLayoutInfo.pSetLayouts = rawDescriptorSetLayouts.data(); // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		pipelineLayout = device->GetDevice().createPipelineLayoutUnique(pipelineLayoutInfo);

		Shared<VulkanRenderPass> rp = static_pointer_cast<VulkanRenderPass>(_desc.renderPass);

		vk::GraphicsPipelineCreateInfo pipelineInfo{};
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
		pipelineInfo.layout = pipelineLayout.get();
		pipelineInfo.renderPass = rp->GetVkRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		vk::ResultValue<vk::UniquePipeline> resultValue = device->GetDevice().createGraphicsPipelineUnique({}, pipelineInfo);
		pipeline = std::move(resultValue.value);
	}

	VulkanPipelineState::~VulkanPipelineState()
	{
	}
	void VulkanPipelineState::Bind() const
	{
		//vkCmdBindDescriptorSets(device->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, descriptorSets.data(), 0, nullptr);
		device->GetCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
	}

	void VulkanPipelineState::CreateShaderStageInfo(const Shared<Shader>& _shader)
	{

	}
	Shared<Material> VulkanPipelineState::CreateMaterial()
	{
		return MakeShared<VulkanMaterial>(device, this);
	}
}
