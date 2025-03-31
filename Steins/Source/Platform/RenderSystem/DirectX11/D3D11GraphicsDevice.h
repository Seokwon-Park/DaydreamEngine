#pragma once

#include "Steins/Render/GraphicsDevice.h"


namespace Steins
{
	class D3D11GraphicsDevice : public GraphicsDevice
	{
	public:
		D3D11GraphicsDevice(SteinsWindow* _window);
		~D3D11GraphicsDevice() override {};

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;
	private:
		bool debugLayerEnabled = false;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;

		Microsoft::WRL::ComPtr<ID3D11Debug> debugLayer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> commandList = nullptr;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory = nullptr;

	};
}

