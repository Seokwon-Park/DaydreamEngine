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

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

		ID3D11Device* GetDevice() const { return device.Get(); }
		ID3D11DeviceContext* GetContext() const { return deviceContext.Get(); }
	private:
		bool debugLayerEnabled = true;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;

		ComPtr<ID3D11Debug> debugLayer = nullptr;
		ComPtr<ID3D11Device> device = nullptr;
		ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

		ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	};
}

