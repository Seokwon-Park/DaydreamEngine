#pragma once

#include "Steins/Graphics/Core/Swapchain.h"
#if defined(STEINS_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Steins/Graphics/Core/RenderDevice.h"
#include "Steins/Core/Window.h"

#include "VulkanFrameBuffer.h"
#include "VulkanRenderDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Steins
{
	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain(VulkanRenderDevice* _device, SteinsWindow* _window, const SwapchainDesc& _desc);
		virtual ~VulkanSwapchain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Shared<Framebuffer> GetBackFramebuffer() { return framebuffers[imageIndex]; };

		VkFormat GetFormat() const { return format; }
		VkSwapchainKHR GetVKSwapchain() const { return swapchain; }
		VkExtent2D GetExtent() const { return extent; }

	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const Array<VkSurfaceFormatKHR>& _availableFormats, RenderFormat _desiredFormat);
		VkPresentModeKHR ChooseSwapPresentMode(const Array<VkPresentModeKHR>& _availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities);

		VulkanRenderDevice* device;
		Array<VkCommandBuffer> commandBuffers;
		Array<Shared<VulkanFramebuffer>> framebuffers;

		VkSurfaceKHR surface; // Vulkan window surface
		VkSwapchainKHR swapchain;
		VkFormat format; // swapchain image format
		VkExtent2D extent;
		Array<VkSemaphore> imageAvailableSemaphores;
		Array<VkSemaphore> renderFinishedSemaphores;
		Array<VkFence> inFlightFences;
		UInt32 imageCount;

		UInt32 currentFrame = 0;
		UInt32 imageIndex = 0;
	};
}