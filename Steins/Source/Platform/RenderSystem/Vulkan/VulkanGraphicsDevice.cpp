#include "SteinsPCH.h"
#include "VulkanGraphicsDevice.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Steins
{
	namespace
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT _messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
			void* _pUserData)
		{
			if (_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			{
				STEINS_CORE_ERROR("Validation layer: {0}", _pCallbackData->pMessage);
			}
			else if (_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) 
			{
				STEINS_CORE_WARN("Validation layer: {0}", _pCallbackData->pMessage);
			}
			else if (_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			{
				STEINS_CORE_INFO("Validation layer: {0}", _pCallbackData->pMessage);
			}
			else if (_messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) 
			{
				STEINS_CORE_TRACE("Validation layer: {0}", _pCallbackData->pMessage);
			}

			return VK_FALSE;
		}

		VkResult CreateDebugUtilsMessengerEXT(VkInstance _instance, const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo, const VkAllocationCallbacks* _pAllocator, VkDebugUtilsMessengerEXT* _pDebugMessenger)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				return func(_instance, _pCreateInfo, _pAllocator, _pDebugMessenger);
			}
			else
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void DestroyDebugUtilsMessengerEXT(VkInstance _instance, VkDebugUtilsMessengerEXT _debugMessenger, const VkAllocationCallbacks* _pAllocator) 
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(_instance, _debugMessenger, _pAllocator);
			}
		}
	}

	VulkanGraphicsDevice::~VulkanGraphicsDevice()
	{
		if (enableValidationLayers == true) 
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroyDevice(device, nullptr);
		vkDestroyInstance(instance, nullptr);
	}
	void VulkanGraphicsDevice::Init()
	{
		CreateInstance();
		SetupDebugMessenger();
#if defined(STEINS_PLATFORM_WINDOWS)
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//createInfo.hwnd = glfwGetWin32Window(window);
		createInfo.hinstance = GetModuleHandle(nullptr);
#endif

		PickPhysicalDevice();
		CreateLogicalDevice();

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

	void VulkanGraphicsDevice::CreateInstance()
	{
		if (enableValidationLayers == true && CheckValidationLayerSupport() == false) 
		{
			STEINS_CORE_ERROR("Validation layers requested, but not available!");
			return;
		}

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


		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<UInt32>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		std::vector<const char*> extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<UInt32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create vkInstance!");
	}
	void VulkanGraphicsDevice::SetupDebugMessenger()
	{
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
		if (result != VK_SUCCESS) 
		{
			STEINS_CORE_ERROR("Failed to set up debug messenger!");
		}
	}
	void VulkanGraphicsDevice::PickPhysicalDevice()
	{
		UInt32 deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			STEINS_CORE_ERROR("Failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const VkPhysicalDevice& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE)
		{
			STEINS_CORE_ERROR("Failed to find a suitable GPU!");
		}
	}
	void VulkanGraphicsDevice::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		Float32 queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 0;

		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = Cast<UInt32>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
		if (result != VK_SUCCESS)
		{
			STEINS_CORE_ERROR("Failed to create logical device!");
		}
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	}

	bool VulkanGraphicsDevice::CheckValidationLayerSupport()
	{
		UInt32 layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}
	std::vector<const char*> VulkanGraphicsDevice::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	void VulkanGraphicsDevice::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo)
	{
		_createInfo = {};
		_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		_createInfo.pfnUserCallback = DebugCallback;
	}
	bool VulkanGraphicsDevice::IsDeviceSuitable(VkPhysicalDevice _device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(_device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(_device, &deviceFeatures);

		QueueFamilyIndices indices = FindQueueFamilies(_device);
		//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		//	deviceFeatures.geometryShader;
		return indices.IsComplete();
	}
	QueueFamilyIndices VulkanGraphicsDevice::FindQueueFamilies(VkPhysicalDevice _device)
	{
		QueueFamilyIndices indices;

		UInt32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) 
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}
			if (indices.IsComplete())
			{
				break;
			}
			i++;
		}

		return indices;
	}

}
