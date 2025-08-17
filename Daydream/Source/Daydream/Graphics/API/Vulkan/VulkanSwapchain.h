#pragma once

#include "Daydream/Graphics/Core/Swapchain.h"
#if defined(DAYDREAM_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "Daydream/Graphics/Core/RenderDevice.h"
#include "Daydream/Core/Window.h"

#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"
#include "VulkanRenderDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Daydream
{
	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain(VulkanRenderDevice* _device, DaydreamWindow* _window, const SwapchainDesc& _desc);
		void CreateSwapchain();
		void CreateCommandBuffers();
		virtual ~VulkanSwapchain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 _height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Shared<Framebuffer> GetBackFramebuffer() { return framebuffers[imageIndex]; };


		inline vk::Format GetFormat() const { return format; }
		inline vk::SwapchainKHR GetVkSwapchain() const { return swapchain.get(); }
		inline vk::Extent2D GetExtent() const { return extent; }
		inline vk::RenderPass GetVkRenderPass() const { return renderPass->GetVkRenderPass(); }
		inline Array<vk::Image> GetSwapchainImages() const { return swapchainImages; };
	private:
		vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const Array<vk::SurfaceFormatKHR>& _availableFormats, RenderFormat _desiredFormat);
		vk::PresentModeKHR ChooseSwapPresentMode(const Array<vk::PresentModeKHR>& _availablePresentModes);
		vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& _capabilities);

		VulkanRenderDevice* device;
		Array<vk::UniqueCommandBuffer> commandBuffers;
		Array<Shared<VulkanFramebuffer>> framebuffers;
		Shared<VulkanRenderPass> renderPass;

		vk::UniqueSurfaceKHR surface; // Vulkan window surface
		vk::UniqueSwapchainKHR swapchain;
		vk::SurfaceFormatKHR surfaceFormat;
		vk::PresentModeKHR presentMode;
		vk::Format format; // swapchain image format
		vk::Extent2D extent;
		Array<vk::UniqueSemaphore> imageAvailableSemaphores;
		Array<vk::UniqueSemaphore> renderFinishedSemaphores;
		Array<vk::UniqueFence> inFlightFences;
		Array<vk::Image> swapchainImages;
		UInt32 imageCount = 0;

		UInt32 currentFrame = 0;
		UInt32 imageIndex = 0;
	};
}