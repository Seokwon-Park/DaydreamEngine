#include "SteinsPCH.h"
#include "D3D11ImGuiRenderer.h"

#include "backends/imgui_impl_dx11.h"

namespace Steins
{
	D3D11ImGuiRenderer::D3D11ImGuiRenderer(D3D11GraphicsDevice* _device)
	{
		device = _device;
	}

	void D3D11ImGuiRenderer::Init(SteinsWindow* _window)
	{
		ImGuiRenderer::Init(_window);
		ImGui_ImplDX11_Init(device->GetDevice(), device->GetContext());
	}

	void D3D11ImGuiRenderer::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGuiRenderer::Shutdown();
	}

	void D3D11ImGuiRenderer::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGuiRenderer::NewFrame();
	}

	void D3D11ImGuiRenderer::Render()
	{
		ImGuiRenderer::Render();
		//device->GetContext()->OMSetRenderTargets(1, , nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
