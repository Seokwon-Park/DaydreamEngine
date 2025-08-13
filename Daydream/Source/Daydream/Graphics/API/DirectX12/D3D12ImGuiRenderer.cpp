#include "DaydreamPCH.h"
#include "D3D12ImGuiRenderer.h"

#include "backends/imgui_impl_dx12.h"

namespace Daydream
{
	D3D12ImGuiRenderer::D3D12ImGuiRenderer(D3D12RenderDevice* _device)
	{
		device = _device;
	}
	void D3D12ImGuiRenderer::Init(DaydreamWindow* _window)
	{
		ImGuiRenderer::Init(_window);
		ImGui_ImplDX12_InitInfo info{};
		info.Device = device->GetDevice();
		info.CommandQueue = device->GetCommandQueue();
		info.NumFramesInFlight = 3;
		info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		info.DSVFormat = DXGI_FORMAT_UNKNOWN;
		info.UserData = device;
		info.SrvDescriptorHeap = device->GetCBVSRVUAVHeap();
		info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo* _info, D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* _outGpuHandle)
			{
				((D3D12RenderDevice*)_info->UserData)->GetCBVSRVUAVHeapAlloc().Alloc(_outCpuHandle, _outGpuHandle);
			};
		info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo* _info, D3D12_CPU_DESCRIPTOR_HANDLE _outCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE _outGpuHandle)
			{
				((D3D12RenderDevice*)_info->UserData)->GetCBVSRVUAVHeapAlloc().Free(_outCpuHandle, _outGpuHandle);
			};

		ImGui_ImplDX12_Init(&info);

	}
	void D3D12ImGuiRenderer::Shutdown()
	{
		ImGui_ImplDX12_Shutdown();
		ImGuiRenderer::Shutdown();
	}
	void D3D12ImGuiRenderer::NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
		ImGuiRenderer::NewFrame();
	}
	void D3D12ImGuiRenderer::Render()
	{
		ImGuiRenderer::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), device->GetCommandList());
	}
}
