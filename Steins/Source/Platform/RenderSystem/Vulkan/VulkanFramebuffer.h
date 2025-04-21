#pragma once


#include "Steins/Render/Framebuffer.h"
#include "Platform/RenderSystem/Vulkan/VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanFramebuffer : public Framebuffer
	{
	public:
	private:
		VkFramebuffer framebuffer;
		VkRenderPass renderpass;
		std::vector<VkImageView> colorImageViews;
	};
}