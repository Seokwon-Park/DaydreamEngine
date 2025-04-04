#pragma once

#include "vulkan/vulkan.h"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	class VulkanGraphicsDevice :public GraphicsDevice
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) override;

	private:
		VkInstance instance; // Vulkan library handle
		VkDebugUtilsMessengerEXT debugMessenger; // Vulkan debug output handle
		VkPhysicalDevice chosenGPU; // GPU chosen as the default device
		VkDevice device; // Vulkan device for commands
		VkSurfaceKHR surface; // Vulkan window surface

	};
}

