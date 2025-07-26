#pragma once

#include "Steins/Graphics/Resources/Texture.h"
#include "D3D11RenderDevice.h"

namespace Steins
{
	class D3D11Texture2D :public Texture2D
	{
	public:
		// constrcuter destructer
		D3D11Texture2D(D3D11RenderDevice* _device, const FilePath& _path, const TextureDesc& _desc);
		D3D11Texture2D(D3D11RenderDevice* _device, ComPtr<ID3D11Texture2D> _texture);
		
		virtual ~D3D11Texture2D();

		virtual void* GetNativeHandle() override { return texture.Get(); }
		virtual void* GetImGuiHandle() { return GetSRV().Get(); };
		ComPtr<ID3D11ShaderResourceView> GetSRV();
		ComPtr<ID3D11DepthStencilView> GetDSV();
		ComPtr<ID3D11RenderTargetView> GetRTV();
		ComPtr<ID3D11UnorderedAccessView> GetUAV();
	protected:

	private:
		D3D11RenderDevice* device;
		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11ShaderResourceView> srv;
		ComPtr<ID3D11DepthStencilView> dsv;
		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11UnorderedAccessView> uav;
	};
}

