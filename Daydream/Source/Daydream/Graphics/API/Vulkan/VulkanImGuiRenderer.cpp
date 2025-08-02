#include "DaydreamPCH.h"
#include "VulkanImGuiRenderer.h"
#include "VulkanSwapchain.h"

#include "backends/imgui_impl_vulkan.h"

namespace Daydream
{
	VulkanImGuiRenderer::VulkanImGuiRenderer(VulkanRenderDevice* _device)
	{
		device = _device;
	}

	void VulkanImGuiRenderer::Init(DaydreamWindow* _window)
	{
		ImGuiRenderer::Init(_window);
		ImGui_ImplVulkan_InitInfo info{};
		//info.ApiVersion =;
		info.Instance = device->GetInstance();
		info.PhysicalDevice = device->GetGPU();
		info.Device = device->GetDevice();
		info.QueueFamily = device->GetGraphicsFamilyIndex();
		info.Queue = device->GetQueue();
		info.DescriptorPool = device->GetDescriptorPool();
		info.RenderPass = static_cast<VulkanSwapchain*>(_window->GetSwapchain())->GetVkRenderPass();
		info.MinImageCount = 2;
		info.ImageCount = 2;
		info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&info);
	}
	void VulkanImGuiRenderer::Shutdown()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGuiRenderer::Shutdown();
	}
	void VulkanImGuiRenderer::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGuiRenderer::NewFrame();
	}
	void VulkanImGuiRenderer::Render()
	{
		ImGuiRenderer::Render();
		//swapChain->FrameRender();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), device->GetCommandBuffer());

	}
}
