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
			createInfo.imageFormat = format;
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

			VkResult result = vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &swapChain);
			if (result != VK_SUCCESS)
			{
				STEINS_CORE_ERROR("Failed to create swapchain");
			}
		}

		{
			//CreataRenderPass
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = format;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;

			VkResult result = vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &renderPass);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create renderpass!");
		}

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

		vkWaitForFences(device->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(device->GetDevice(), 1, &inFlightFence);
		vkAcquireNextImageKHR(device->GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		backFramebuffer = MakeShared<VulkanFramebuffer>(device, this);

	}
	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDeviceWaitIdle(device->GetDevice());
		vkDestroySemaphore(device->GetDevice(), imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(device->GetDevice(), renderFinishedSemaphore, nullptr);
		vkDestroyFence(device->GetDevice(), inFlightFence, nullptr);
		vkDestroyRenderPass(device->GetDevice(), renderPass, nullptr);
		vkDestroySwapchainKHR(device->GetDevice(), swapChain, nullptr);
		vkDestroySurfaceKHR(device->GetInstance(), surface, nullptr);
	}
	void VulkanSwapChain::SetVSync(bool _enabled)
	{
	}
	void VulkanSwapChain::SwapBuffers()
	{
		VkCommandBuffer cmdbuf = device->GetCommandBuffer();
		//vkWaitForFences(device->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		//vkResetFences(device->GetDevice(), 1, &inFlightFence);
		//vkAcquireNextImageKHR(device->GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		//vkResetCommandBuffer(cmdbuf, 0);
		recordCommandBuffer(cmdbuf, imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdbuf;

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

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(device->GetQueue(), &presentInfo);
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
		/*VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VkResult result = vkBeginCommandBuffer(_commandBuffer, &beginInfo);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to begin recording command buffer!");*/

		//VkFramebuffer buf = ((VulkanFramebuffer*)backFramebuffer.get())->GetFrameBuffers()[_imageIndex];

		//VkRenderPassBeginInfo renderPassInfo{};
		//renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		//renderPassInfo.renderPass = renderPass;
		//renderPassInfo.framebuffer = buf;
		//renderPassInfo.renderArea.offset = { 0, 0 };
		//renderPassInfo.renderArea.extent = extent;

		//VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		//renderPassInfo.clearValueCount = 1;
		//renderPassInfo.pClearValues = &clearColor;

		//vkCmdBeginRenderPass(_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)extent.width;
		viewport.height = (float)extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;
		vkCmdSetScissor(_commandBuffer, 0, 1, &scissor);

//		vkCmdDraw(_commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(_commandBuffer);

		if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	void VulkanSwapChain::FrameRender()
	{
		VkResult err = vkAcquireNextImageKHR(device->GetDevice(), swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		vkWaitForFences(device->GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking

		vkResetFences(device->GetDevice(), 1, &inFlightFence);
		{
			err = vkResetCommandPool(device->GetDevice(), device->GetCommandPool(), 0);
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(device->GetCommandBuffer(), &info);
		}
		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = renderPass;
			info.framebuffer = ((VulkanFramebuffer*)backFramebuffer.get())->GetFrameBuffers()[imageIndex];
			info.renderArea.extent.width = extent.width;
			info.renderArea.extent.height = extent.height;
			info.clearValueCount = 1;
			VkClearValue clearColor = { {{1.0f, 1.0f, 1.0f, 1.0f}} };
			info.pClearValues = &clearColor;
			vkCmdBeginRenderPass(device->GetCommandBuffer(), &info, VK_SUBPASS_CONTENTS_INLINE);
		}
	}

	void VulkanSwapChain::EndRender()
	{
		// Submit command buffer
		vkCmdEndRenderPass(device->GetCommandBuffer());
		{
			VkCommandBuffer buf = device->GetCommandBuffer();
			VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			VkSubmitInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &imageAvailableSemaphore;
			info.pWaitDstStageMask = &wait_stage;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &buf;
			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &renderFinishedSemaphore;

			vkEndCommandBuffer(device->GetCommandBuffer());
			vkQueueSubmit(device->GetQueue(), 1, &info, inFlightFence);
		}
	}
}
