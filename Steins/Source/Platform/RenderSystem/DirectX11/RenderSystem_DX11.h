#pragma once

#include "Platform/RenderSystem/RenderSystem.h"
#include "Steins/Window.h"

using namespace Microsoft::WRL;

namespace Steins
{
	class RenderSystem_DX11 : public RenderSystem
	{
	public:
		RenderSystem_DX11(SteinsWindow* _Window);
		~RenderSystem_DX11() override {};

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;
	private:
		ComPtr<ID3D11Device> Device;
		ComPtr<ID3D11DeviceContext> Context;

	};
}

