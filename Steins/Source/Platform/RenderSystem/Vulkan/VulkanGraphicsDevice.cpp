#include "SteinsPCH.h"
#include "VulkanGraphicsDevice.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	VulkanGraphicsDevice::~VulkanGraphicsDevice()
	{
		vkDestroyInstance(instance, nullptr);
	}
	void VulkanGraphicsDevice::Init()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "My Vulkan App";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	}
	void VulkanGraphicsDevice::Shutdown()
	{
	}

	void VulkanGraphicsDevice::Render()
	{
	}

	void VulkanGraphicsDevice::SwapBuffers()
	{
	}
}
