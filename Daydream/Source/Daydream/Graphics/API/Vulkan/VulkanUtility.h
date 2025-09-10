#pragma once

#include "vulkan/vulkan.hpp"

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

#include "Daydream/Graphics/Resources/Sampler.h"
#include "Daydream/Graphics/States/RasterizerState.h"



namespace Daydream::GraphicsUtility::Vulkan
{
	vk::Format ConvertRenderFormatToVkFormat(RenderFormat _format);
	vk::ShaderStageFlagBits ConvertToShaderStageFlagBit(ShaderType _type);
	vk::ImageUsageFlags ConvertToVkImageUsageFlags(RenderBindFlags usageFlags);

	constexpr vk::SamplerAddressMode ConvertToVkAddressMode(WrapMode _wrapMode);
	constexpr vk::Filter ConvertToVkFilter(FilterMode _filter);
	constexpr vk::SamplerMipmapMode ConvertToVkMipmapMode(FilterMode _mipMapFilter);
	constexpr vk::CompareOp ConvertToVkCompareOp(ComparisonFunc _func);

	vk::SamplerCreateInfo TranslateToVulkanSamplerCreateInfo(const SamplerDesc& _desc);

	constexpr vk::CullModeFlags ConvertToVulkanCullMode(const CullMode& _cullMode);
	constexpr vk::PolygonMode ConvertToVulkanFillMode(const FillMode& _fillMode);

	vk::PipelineRasterizationStateCreateInfo TranslateToVulkanRasterizerCreateInfo(const RasterizerStateDesc& _desc);

}