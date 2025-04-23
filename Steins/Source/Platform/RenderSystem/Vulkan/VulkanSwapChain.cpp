#include "SteinsPCH.h"
#include "VulkanSwapChain.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	VulkanSwapChain::VulkanSwapChain(VulkanGraphicsDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		device = _device;
		desc = *_desc;
		GLFWwindow* window = Cast<GLFWwindow>(_window->GetNativeWindow());
		//#if defined(STEINS_PLATFORM_WINDOWS)
		//		VkWin32SurfaceCreateInfoKHR createInfo{};
		//		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//		createInfo.hwnd = glfwGetWin32Window(window);
		//		createInfo.hinstance = GetModuleHandle(nullptr);
		//#endif
		{
			VkResult result = glfwCreateWindowSurface(device->GetInstance(), window, nullptr, &surface);
			if (result != VK_SUCCESS) {
				STEINS_CORE_ERROR("Failed to create window surface!");
			}
		}

		{

			SwapChainSupportDetails swapChainSupport = device->QuerySwapChainSupport(surface);
			if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
			{
				STEINS_CORE_ERROR("GPU has no supported formats or presentmodes");
			}


			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats, _desc->format);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
			format = surfaceFormat.format;
			extent = ChooseSwapExtent(swapChainSupport.capabilities);

			UInt32 imageCount = desc.bufferCount;

			// 서피스 제한 확인
			if (imageCount < swapChainSupport.capabilities.minImageCount)
			{
				imageCount = swapChainSupport.capabilities.minImageCount;
			}

			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
			{
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = surface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			createInfo.oldSwapchain = VK_NULL_HANDLE;

			VkResult result = vkCreateSwapchainKHR(device->GetLogicalDevice(), &createInfo, nullptr, &swapChain);
			if (result != VK_SUCCESS)
			{
				STEINS_CORE_ERROR("Failed to create swapchain");
			}
		}

		backFramebuffer = MakeShared<VulkanFramebuffer>(device, this);

	}
	VulkanSwapChain::~VulkanSwapChain()
	{

		vkDestroySwapchainKHR(device->GetLogicalDevice(), swapChain, nullptr);
		vkDestroySurfaceKHR(device->GetInstance(), surface, nullptr);
	}
	void VulkanSwapChain::SetVSync(bool _enabled)
	{
	}
	void VulkanSwapChain::SwapBuffers()
	{
		
	}

	VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, RenderFormat _desiredFormat)
	{
		VkFormat desiredFormat = GraphicsUtil::RenderFormatToVkFormat(_desiredFormat);
		for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
		{
			if (availableFormat.format == desiredFormat && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}
	VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const VkPresentModeKHR& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = {
				Cast<UInt32>(desc.width),
				Cast<UInt32>(desc.height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}
}
