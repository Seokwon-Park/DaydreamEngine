#pragma once

#include "Daydream/Enum/RendererEnums.h"
#include "vulkan/vulkan.hpp"

#include "Daydream/Graphics/Utility/GraphicsUtility.h"

namespace Daydream::GraphicsUtility::Vulkan
{
	vk::Format ConvertRenderFormatToVkFormat(RenderFormat _format);
	vk::ShaderStageFlagBits ConvertToShaderStageFlagBit(ShaderType _type);
	vk::ImageUsageFlags ConvertToVkImageUsageFlags(RenderBindFlags usageFlags);

}