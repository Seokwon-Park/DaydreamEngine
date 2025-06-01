#pragma once

#include "vulkan/vulkan.h"
#include "Steins/Render/RenderPass.h"

namespace Steins
{
	class VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(Vulkan);

	private:
		VkRenderPass renderPass;
	};
}