#pragma once


#include "Steins/Render/Framebuffer.h"
#include "Platform/RenderSystem/Vulkan/VulkanGraphicsDevice.h"

namespace Steins
{
	class VulkanSwapChain;

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(VulkanGraphicsDevice* _device, const FramebufferDesc& _spec);
		VulkanFramebuffer(VulkanGraphicsDevice* _device, VulkanSwapChain* _swapChain);

		virtual ~VulkanFramebuffer() override;

		virtual void Bind() const override;
		virtual void Clear(Color _color) override;

		std::vector<VkFramebuffer>& GetFrameBuffers() { return framebuffers; }

	private:
		VulkanGraphicsDevice* device;
		std::vector<VkFramebuffer> framebuffers;
		std::vector<VkImage> colorImages;
		std::vector<VkImageView> colorImageViews;
		VkImage depthImage;
		VkImageView depthStencilViews;
	};
}