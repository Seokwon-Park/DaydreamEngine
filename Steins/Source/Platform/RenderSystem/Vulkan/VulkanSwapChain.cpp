#include "SteinsPCH.h"
#include "VulkanSwapChain.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	VulkanSwapChain::VulkanSwapChain(VulkanRenderDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window)
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
			SwapChainSupportDetails SwapchainSupport = device->QuerySwapChainSupport(surface);
			if (SwapchainSupport.formats.empty() || SwapchainSupport.presentModes.empty())
			{
				STEINS_CORE_ERROR("GPU has no supported formats or presentmodes");
			}


			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(SwapchainSupport.formats, _desc->format);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(SwapchainSupport.presentModes);
			format = surfaceFormat.format;
			extent = ChooseSwapExtent(SwapchainSupport.capabilities);

			UInt32 imageCount = desc.bufferCount;

			// 서피스 제한 확인
			if (imageCount < SwapchainSupport.capabilities.minImageCount)
			{
				imageCount = SwapchainSupport.capabilities.minImageCount;
			}

			if (SwapchainSupport.capabilities.maxImageCount > 0 && imageCount > SwapchainSupport.capabilities.maxImageCount)
			{
				imageCount = SwapchainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = surface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.preTransform = SwapchainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			createInfo.oldSwapchain = VK_NULL_HANDLE;

			VkResult result = vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &swapChain);
			if (result != VK_SUCCESS)
			{
				STEINS_CORE_ERROR("Failed to create Swapchain");
			}
		}
		device->AddSwapChain(this);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(device->GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(device->GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(device->GetDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}

		framebuffers.resize(desc.bufferCount);
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			imageIndex = i;
			framebuffers[i] = MakeShared<VulkanFramebuffer>(device, this);
		}
		imageIndex = 0;

		vkWaitForFences(device->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(device->GetDevice(), 1, &inFlightFence);
		vkAcquireNextImageKHR(device->GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		vkResetCommandBuffer(device->GetCommandBuffer() , 0);
		//fb->Bind();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VkResult result = vkBeginCommandBuffer(device->GetCommandBuffer(), &beginInfo);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to begin recording command buffer!");

		//VkFramebuffer buf = ((VulkanFramebuffer*)backFramebuffer.get())->GetFrameBuffers()[imageIndex];

		framebuffers[imageIndex]->Begin();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = (float)extent.height;
		viewport.width = (float)extent.width;
		viewport.height = -(float)extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(device->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;
		vkCmdSetScissor(device->GetCommandBuffer(), 0, 1, &scissor);

	}
	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDeviceWaitIdle(device->GetDevice());
		vkDestroySemaphore(device->GetDevice(), imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(device->GetDevice(), renderFinishedSemaphore, nullptr);
		vkDestroyFence(device->GetDevice(), inFlightFence, nullptr);
		vkDestroySwapchainKHR(device->GetDevice(), swapChain, nullptr);
		vkDestroySurfaceKHR(device->GetInstance(), surface, nullptr);
	}
	void VulkanSwapChain::SetVSync(bool _enabled)
	{
	}
	void VulkanSwapChain::SwapBuffers()
	{
		framebuffers[imageIndex]->End();
		//vkCmdEndRenderPass(cmdbuf);
		if (vkEndCommandBuffer(device->GetCommandBuffer()) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		VkCommandBuffer buffers[] = { device->GetCommandBuffer() };
		submitInfo.pCommandBuffers = buffers;

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(device->GetQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR Swapchains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = Swapchains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(device->GetQueue(), &presentInfo);
		vkWaitForFences(device->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(device->GetDevice(), 1, &inFlightFence);
		vkAcquireNextImageKHR(device->GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		vkResetCommandBuffer(device->GetCommandBuffer(), 0);
		recordCommandBuffer(device->GetCommandBuffer(), imageIndex);
	}

	void VulkanSwapChain::ResizeSwapChain(UInt32 _width, UInt32 height)
	{
	}

	VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats, RenderFormat _desiredFormat)
	{
		VkFormat desiredFormat = GraphicsUtil::RenderFormatToVkFormat(_desiredFormat);
		for (const VkSurfaceFormatKHR& availableFormat : _availableFormats)
		{
			if (availableFormat.format == desiredFormat && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return _availableFormats[0];
	}
	VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes)
	{
		for (const VkPresentModeKHR& availablePresentMode : _availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities)
	{
		if (_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return _capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = {
				Cast<UInt32>(desc.width),
				Cast<UInt32>(desc.height)
			};

			actualExtent.width = std::clamp(actualExtent.width, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}
	void VulkanSwapChain::recordCommandBuffer(VkCommandBuffer _commandBuffer, UInt32 _imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VkResult result = vkBeginCommandBuffer(_commandBuffer, &beginInfo);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to begin recording command buffer!");

		//VkFramebuffer buf = ((VulkanFramebuffer*)backFramebuffer.get())->GetFrameBuffers()[_imageIndex];

		//VkRenderPassBeginInfo renderPassInfo{};
		//renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		//renderPassInfo.renderPass = device->GetMainRenderPass();
		//renderPassInfo.framebuffer = buf;
		//renderPassInfo.renderArea.offset = { 0, 0 };
		//renderPassInfo.renderArea.extent = extent;

		//VkClearValue clearColor = { {1.0f, 1.0f, 1.0f, 1.0f} };
		//renderPassInfo.clearValueCount = 1;
		//renderPassInfo.pClearValues = &clearColor;

		//vkCmdBeginRenderPass(_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		framebuffers[_imageIndex]->Begin();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = (float)extent.height;
		viewport.width = (float)extent.width;
		viewport.height = -(float)extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;
		vkCmdSetScissor(_commandBuffer, 0, 1, &scissor);

//		vkCmdDraw(_commandBuffer, 3, 1, 0, 0);


	}
}
