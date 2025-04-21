#pragma once
#include "Steins/Core/Window.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_metal.h>

namespace Steins
{
	class ImGuiHelper
	{
	public:
		static void Init();
		static void Shutdown();
		static void NewFrame();
		static void Render();
	};
}

