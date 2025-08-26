#include "DaydreamPCH.h"
#include "VulkanSwapchain.h"

#include "VulkanUtility.h"

namespace Daydream
{
	VulkanSwapchain::VulkanSwapchain(VulkanRenderDevice* _device, DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		device = _device;
		desc = _desc;
		GLFWwindow* window = Cast<GLFWwindow>(_window->GetNativeWindow());
		//#if defined(DAYDREAM_PLATFORM_WINDOWS)
		//		VkWin32SurfaceCreateInfoKHR createInfo{};
		//		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//		createInfo.hwnd = glfwGetWin32Window(window);
		//		createInfo.hinstance = GetModuleHandle(nullptr);
		//#endif
		VkSurfaceKHR tempSurface;
		VkResult result = glfwCreateWindowSurface(device->GetInstance(), window, nullptr, &tempSurface);
		if (result != VK_SUCCESS)
		{
			DAYDREAM_CORE_ERROR("Failed to create window surface!");
		}

		surface = vk::UniqueSurfaceKHR(tempSurface, vk::detail::ObjectDestroy<vk::Instance, vk::detail::DispatchLoaderDynamic>(device->GetInstance()));

		CreateSwapchain();
		CreateCommandBuffers();

		vk::SemaphoreCreateInfo semaphoreInfo{};

		vk::FenceCreateInfo fenceInfo{};
		fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		imageAvailableSemaphores.resize(imageCount);
		renderFinishedSemaphores.resize(imageCount);
		inFlightFences.resize(imageCount);
		for (UInt32 i = 0; i < imageCount; i++)
		{
			imageAvailableSemaphores[i] = device->GetDevice().createSemaphoreUnique(semaphoreInfo);
			renderFinishedSemaphores[i] = device->GetDevice().createSemaphoreUnique(semaphoreInfo);
			inFlightFences[i] = device->GetDevice().createFenceUnique(fenceInfo);
		}

		RenderPassAttachmentDesc colorDesc;
		colorDesc.format = _desc.format;
		colorDesc.isSwapchain = true;

		RenderPassDesc rpDesc{};
		rpDesc.colorAttachments.push_back(colorDesc);

		renderPass = MakeShared<VulkanRenderPass>(device, rpDesc);
		mainRenderPass = renderPass;

		framebuffers.resize(desc.bufferCount);
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			framebuffers[i] = MakeShared<VulkanFramebuffer>(device, this, renderPass.get(), swapchainImages[i]);
		}
		BeginFrame();
		//framebuffers[imageIndex]->Begin();
	}
	void VulkanSwapchain::CreateSwapchain()
	{
		SwapchainSupportDetails SwapchainSupport = device->QuerySwapchainSupport(*surface);
		if (SwapchainSupport.formats.empty() || SwapchainSupport.presentModes.empty())
		{
			DAYDREAM_CORE_ERROR("GPU has no supported formats or presentmodes");
		}

		surfaceFormat = ChooseSwapSurfaceFormat(SwapchainSupport.formats, desc.format);
		presentMode = ChooseSwapPresentMode(SwapchainSupport.presentModes);
		format = surfaceFormat.format;
		extent = ChooseSwapExtent(SwapchainSupport.capabilities);

		imageCount = desc.bufferCount;

		// ���ǽ� ���� Ȯ��
		if (imageCount < SwapchainSupport.capabilities.minImageCount)
		{
			imageCount = SwapchainSupport.capabilities.minImageCount;
		}

		if (SwapchainSupport.capabilities.maxImageCount > 0 && imageCount > SwapchainSupport.capabilities.maxImageCount)
		{
			imageCount = SwapchainSupport.capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR createInfo{};
		createInfo.surface = *surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.preTransform = SwapchainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = *swapchain;

		vk::SwapchainKHR newSwapchain;

		newSwapchain = device->GetDevice().createSwapchainKHR(createInfo);
		
		swapchain.release();

		swapchain = vk::UniqueSwapchainKHR(newSwapchain, vk::detail::ObjectDestroy<vk::Device, vk::detail::DispatchLoaderDynamic>(device->GetDevice()));

		swapchainImages = device->GetDevice().getSwapchainImagesKHR(swapchain.get());

	}
	void VulkanSwapchain::CreateCommandBuffers()
	{
		commandBuffers.resize(imageCount);
		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.commandPool = device->GetCommandPool();
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = commandBuffers.size();

		commandBuffers = device->GetDevice().allocateCommandBuffersUnique(allocInfo);
	}
	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDeviceWaitIdle(device->GetDevice());
		//for (UInt32 i = 0; i < imageCount; i++)
		//{
		//	vkDestroySemaphore(device->GetDevice(), imageAvailableSemaphores[i], nullptr);
		//	vkDestroySemaphore(device->GetDevice(), renderFinishedSemaphores[i], nullptr);
		//	vkDestroyFence(device->GetDevice(), inFlightFences[i], nullptr);
		//}

		//vkDestroySwapchainKHR(device->GetDevice(), swapchain, nullptr);
		//vkDestroySurfaceKHR(device->GetInstance(), surface, nullptr);
	}
	void VulkanSwapchain::SetVSync(bool _enabled)
	{
	}
	void VulkanSwapchain::SwapBuffers()
	{
		EndFrame();

		BeginFrame();
		
		ResizeFramebuffers();

		//framebuffers[currentFrame]->Begin();
	}

	void VulkanSwapchain::ResizeSwapchain(UInt32 _width, UInt32 _height)
	{
		// 1. ��� GPU �۾� �Ϸ� ���
		vkDeviceWaitIdle(device->GetDevice());

		EndFrame();

		desc.width = _width;
		desc.height = _height;
		framebuffers.clear();

		CreateSwapchain();

		framebuffers.resize(desc.bufferCount);
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			framebuffers[i] = MakeShared<VulkanFramebuffer>(device, this, renderPass.get(), swapchainImages[i]);
		}

		currentFrame = (currentFrame + 1) % imageCount;

		BeginFrame();

		//VkViewport viewport{};
		////viewport.x = 0.0f;
		////viewport.y = (float)extent.height;
		////viewport.width = (float)extent.width;
		////viewport.height = -(float)extent.height;
		//viewport.x = 0.0f;
		//viewport.y = 0.0f;
		//viewport.width = (float)extent.width;
		//viewport.height = (float)extent.height;
		//viewport.minDepth = 0.0f;
		//viewport.maxDepth = 1.0f;
		//vkCmdSetViewport(device->GetCommandBuffer(), 0, 1, &viewport);

		//VkRect2D scissor{};
		//scissor.offset = { 0, 0 };
		//scissor.extent = extent;
		//vkCmdSetScissor(device->GetCommandBuffer(), 0, 1, &scissor);
	}

	void VulkanSwapchain::BeginFrame()
	{
		//���� �������� GPU �۾� �Ϸ�ƴٴ� ��ȣ�� inFlightFence�� �ޱ�� �ϰ� ���
		auto result = device->GetDevice().waitForFences(1, &inFlightFences[currentFrame].get(), VK_FALSE, UINT64_MAX);

		//�Ϸ� ������ �潺 ���´� ��ȣ�ޱ� ������
		result = device->GetDevice().resetFences(1, &inFlightFences[currentFrame].get());

		//�̹����� GPU�� ��û. ��밡���� �̹����� �ε����� imageIndex�� �����ϰ� imageAvailableSemaphore�� ��ȣ�� �����϶�� ���
		result = device->GetDevice().acquireNextImageKHR(swapchain.get(), UINT64_MAX, imageAvailableSemaphores[currentFrame].get(), VK_NULL_HANDLE, &imageIndex);

		//�̹��� ��û�� �س��� �ϴ� Ŀ�ǵ� �ޱ� ����
		device->SetCommandBuffer(commandBuffers[currentFrame].get());
		commandBuffers[currentFrame]->reset({});
		//fb->Bind();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		commandBuffers[currentFrame]->begin(beginInfo);
	}

	void VulkanSwapchain::EndFrame()
	{
		commandBuffers[currentFrame]->end();

		vk::SubmitInfo submitInfo{};

		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		//�� ��������鿡 ��ȣ�� �� �;� Submit�� �Ѵ�.
		submitInfo.pWaitSemaphores = &imageAvailableSemaphores[currentFrame].get();
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		submitInfo.pCommandBuffers = &commandBuffers[currentFrame].get();

		//�������� ������ ����ٰ� ��ȣ�� ������.
		//VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderFinishedSemaphores[currentFrame].get();

		vk::Result result = device->GetGraphicsQueue().submit(1, &submitInfo, inFlightFences[currentFrame].get());

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphores[currentFrame].get();

		//vk::SwapchainKHR Swapchains[] = { swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain.get();
		presentInfo.pImageIndices = &imageIndex;

		result = device->GetGraphicsQueue().presentKHR(presentInfo);

		currentFrame = (currentFrame + 1) % imageCount;
	}

	vk::SurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const Array<vk::SurfaceFormatKHR>& _availableFormats, RenderFormat _desiredFormat)
	{
		vk::Format desiredFormat = GraphicsUtility::Vulkan::ConvertRenderFormatToVkFormat(_desiredFormat);
		for (const vk::SurfaceFormatKHR& availableFormat : _availableFormats)
		{
			if (availableFormat.format == desiredFormat && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			{
				return availableFormat;
			}
		}

		return _availableFormats[0];
	}
	vk::PresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const Array<vk::PresentModeKHR>& _availablePresentModes)
	{
		for (const vk::PresentModeKHR& availablePresentMode : _availablePresentModes)
		{
			if (availablePresentMode == vk::PresentModeKHR::eMailbox)
			{
				return availablePresentMode;
			}
		}

		return vk::PresentModeKHR::eFifo;
	}
	vk::Extent2D VulkanSwapchain::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& _capabilities)
	{
		if (_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return _capabilities.currentExtent;
		}
		else {
			vk::Extent2D actualExtent = {
				Cast<UInt32>(desc.width),
				Cast<UInt32>(desc.height)
			};

			actualExtent.width = std::clamp(actualExtent.width, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}
}
