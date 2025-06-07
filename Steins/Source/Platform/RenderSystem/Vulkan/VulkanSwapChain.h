#pragma once

#include "Steins/Render/SwapChain.h"
#if defined(STEINS_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Steins/Render/RenderDevice.h"
#include "Steins/Core/Window.h"

#include "VulkanFrameBuffer.h"
#include "VulkanRenderDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Steins
{
	class VulkanSwapChain : public SwapChain
	{
	public:
		VulkanSwapChain(VulkanRenderDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window);
		virtual ~VulkanSwapChain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapChain(UInt32 _width, UInt32 height) override;

		virtual Framebuffer* GetBackFramebuffer() { return framebuffers[imageIndex].get(); };

		VkFormat GetFormat() const { return format; }
		VkSwapchainKHR GetVKSwapChain() const { return swapChain; }
		VkExtent2D GetExtent() const { return extent; }
		UInt32 GetBackbufferIndex() const { return imageIndex; }

		void recordCommandBuffer(VkCommandBuffer _commandBuffer, UInt32 _imageIndex);



	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats, RenderFormat _desiredFormat);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities);

		VulkanRenderDevice* device;
		std::vector<Shared<VulkanFramebuffer>> framebuffers;

		VkSurfaceKHR surface; // Vulkan window surface
		VkSwapchainKHR swapChain;
		VkFormat format; // swapchain image format
		VkExtent2D extent;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;


		//temp
		UInt32 imageIndex = 0;
	};
}