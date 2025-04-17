#pragma once

#include "vulkan/vulkan.h"
#include "vulkan/vulkan.hpp"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	struct QueueFamilyIndices {
		std::optional<UInt32> graphicsFamily;
		//std::optional<UInt32> presentFamily; // 지금은 필요없는듯.

		bool IsComplete() {
			return graphicsFamily.has_value();
		}
	};

	class VulkanGraphicsDevice :public GraphicsDevice
	{
	public:


		virtual ~VulkanGraphicsDevice() override;

		virtual void Init() override;
		
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;

		VkInstance GetInstance() const { return instance; }

	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateLogicalDevice();
		void PickPhysicalDevice();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
		bool IsDeviceSuitable(VkPhysicalDevice _device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice _device);

#if defined(STEINS_DEBUG) || defined(_DEBUG)
		const bool enableValidationLayers = true;
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#else
		const bool enableValidationLayers = false;
		const std::vector<const char*> validationLayers = {};
#endif
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		VkInstance instance; // Vulkan library handle
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // GPU chosen as the default device
		VkDevice device; // Vulkan device for commands
		VkQueue graphicsQueue; // vulkan graphics Queue

	};
}

