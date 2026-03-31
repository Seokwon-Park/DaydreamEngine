#include "DaydreamPCH.h"
#include "OpenGLImGuiRenderer.h"

#include "backends/imgui_impl_opengl3.h"


namespace Daydream
{
	OpenGLImGuiRenderer::OpenGLImGuiRenderer(OpenGLRenderDevice* _device)
	{
		device = _device;
	}

	void OpenGLImGuiRenderer::Init(DaydreamWindow* _window)
	{
		ImGuiRenderer::Init(_window);
		ImGui_ImplOpenGL3_Init(device->GetVersion().c_str());
	}

	void OpenGLImGuiRenderer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGuiRenderer::Shutdown();
	}

	void OpenGLImGuiRenderer::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGuiRenderer::NewFrame();
	}

	void OpenGLImGuiRenderer::RenderDrawData(RenderCommandList* _activeCommandList, ImDrawData* _drawData)
	{
		ImGuiRenderer::RenderDrawData(_activeCommandList, _drawData);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
