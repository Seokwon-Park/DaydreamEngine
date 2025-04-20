#pragma once

#if defined(STEINS_PLATFORM_WINDOWS)
	#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Steins/Render/SwapChain.h"
#include "Steins/Render/GraphicsDevice.h"
#include "Steins/Core/Window.h"

#include "Platform/RenderSystem/Vulkan/VulkanGraphicsDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Steins
{
	class VulkanSwapChain : public SwapChain
	{
	public:
		VulkanSwapChain(GraphicsDevice* _device, SwapChainDesc* _desc, SteinsWindow* _window);
		virtual ~VulkanSwapChain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, RenderFormat _desiredFormat);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);


		VulkanGraphicsDevice* device;
		VkSurfaceKHR surface; // Vulkan window surface
		VkSwapchainKHR swapChain;
		VkFormat format;
		VkExtent2D extent;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

	};
}