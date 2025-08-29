#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "vulkan/vulkan.hpp"

#include "Daydream/Graphics/States/RasterizerState.h"

#include "Daydream/Graphics/Utility/GraphicsUtility.h"


namespace Daydream::GraphicsUtility::Vulkan
{
	vk::Format ConvertRenderFormatToVkFormat(RenderFormat _format);
	vk::ShaderStageFlagBits ConvertToShaderStageFlagBit(ShaderType _type);
	vk::ImageUsageFlags ConvertToVkImageUsageFlags(RenderBindFlags usageFlags);

	constexpr vk::CullModeFlags ConvertToVulkanCullMode(const CullMode& _cullMode);
	constexpr vk::PolygonMode ConvertToVulkanFillMode(const FillMode& _fillMode);


	vk::PipelineRasterizationStateCreateInfo TranslateToVulkanRasterizerCreateInfo(const RasterizerStateDesc& _desc);

}