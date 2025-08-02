#pragma once

#include "D3D11RenderDevice.h"
#include "Daydream/ImGui/ImGuiRenderer.h"

namespace Daydream
{

class D3D11ImGuiRenderer: public ImGuiRenderer
{
public:
	D3D11ImGuiRenderer(D3D11RenderDevice* _device);

	virtual void Init(DaydreamWindow* _window) override;
	virtual void Shutdown() override;
	virtual void NewFrame() override;
	virtual void Render() override;
private:
	D3D11RenderDevice* device;
};
}

