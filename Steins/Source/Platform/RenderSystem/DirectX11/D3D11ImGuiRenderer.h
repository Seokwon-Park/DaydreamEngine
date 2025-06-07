#pragma once

#include "D3D11RenderDevice.h"
#include "Steins/ImGui/ImGuiRenderer.h"

namespace Steins
{

class D3D11ImGuiRenderer: public ImGuiRenderer
{
public:
	D3D11ImGuiRenderer(D3D11RenderDevice* _device);

	virtual void Init(SteinsWindow* _window) override;
	virtual void Shutdown() override;
	virtual void NewFrame() override;
	virtual void Render() override;
private:
	D3D11RenderDevice* device;
};
}

