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
		swapchain = Cast<VulkanSwapchain*>(_window->GetSwapchain());
		ImGui_ImplVulkan_InitInfo info{};
		//info.ApiVersion =;
		info.Instance = device->GetInstance();
		info.PhysicalDevice = device->GetPhysicalDevice();
		info.Device = device->GetDevice();
		info.QueueFamily = device->GetGraphicsFamilyIndex();
		info.Queue = device->GetGraphicsQueue();
		info.DescriptorPool = device->GetDescriptorPool();
		info.RenderPass = swapchain->GetVkRenderPass();
		info.MinImageCount = 3;
		info.ImageCount = 3;
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
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Cast<VulkanRenderCommandList*>(swapchain->GetRenderCommandList().get())->GetVkCommandBuffer());
	}
}
