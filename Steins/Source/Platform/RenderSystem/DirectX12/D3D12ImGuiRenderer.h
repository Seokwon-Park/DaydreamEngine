#pragma once

#include "Steins/ImGui/ImGuiRenderer.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12ImGuiRenderer :public ImGuiRenderer
	{
	public:
		D3D12ImGuiRenderer(D3D12GraphicsDevice* _device);

		virtual void Init(SteinsWindow* _window);
		virtual void Shutdown();
		virtual void NewFrame();
		virtual void Render();
	private:
		D3D12GraphicsDevice* device;
	};
}