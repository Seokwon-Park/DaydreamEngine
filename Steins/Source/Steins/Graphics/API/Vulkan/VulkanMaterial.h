#pragma once

#include "Steins/Graphics/Resources/Material.h"
#include "Steins/Graphics/API/Vulkan/VulkanRenderDevice.h"
#include "Steins/Graphics/API/Vulkan/VulkanPipelineState.h"

namespace Steins
{
	class VulkanMaterial : public Material
	{
		VulkanMaterial(VulkanRenderDevice* _device);
	};
}