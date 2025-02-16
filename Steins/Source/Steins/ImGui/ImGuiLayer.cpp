#include "SteinsPCH.h"
#include "ImGuiLayer.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_dx11.h"

#include "imgui.h"



namespace Steins
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
	}
	void ImGuiLayer::OnEvent(Event& _event)
	{
	}
}

