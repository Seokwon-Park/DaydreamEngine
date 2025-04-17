#include "SteinsPCH.h"
#include "VulkanSwapChain.h"

namespace Steins
{
	VulkanSwapChain::VulkanSwapChain(GraphicsDevice* _device, SwapChainDesc* _desc, SteinsWindow* _window)
	{
		device = Cast<VulkanGraphicsDevice>(_device);
		GLFWwindow* window = Cast<GLFWwindow>(_window->GetNativeWindow());
//#if defined(STEINS_PLATFORM_WINDOWS)
//		VkWin32SurfaceCreateInfoKHR createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//		createInfo.hwnd = glfwGetWin32Window(window);
//		createInfo.hinstance = GetModuleHandle(nullptr);
//#endif

		VkResult result = glfwCreateWindowSurface(device->GetInstance(), window, nullptr, &surface);
		if (result != VK_SUCCESS) {
			STEINS_CORE_ERROR("Failed to create window surface!");
		}
	}
	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDestroySurfaceKHR(device->GetInstance(), surface, nullptr);
	}
}
