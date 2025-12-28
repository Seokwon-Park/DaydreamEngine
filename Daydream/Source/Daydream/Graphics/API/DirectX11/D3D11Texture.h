#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "D3D11RenderDevice.h"
#include "D3D11Sampler.h"
#include "D3D11Common.h"

namespace Daydream
{
	class D3D11Texture2D :public Texture2D
	{
	public:
		// constrcuter destructer
		D3D11Texture2D(D3D11RenderDevice* _device, const TextureDesc& _desc, const void* _initialData = nullptr);
		//D3D11Texture2D(D3D11RenderDevice* _device, const FilePath& _path, const TextureDesc& _desc);
		//D3D11Texture2D(D3D11RenderDevice* _device, ComPtr<ID3D11Texture2D> _texture);
		virtual ~D3D11Texture2D();

		virtual void SetSampler(Shared<Sampler> _sampler) override;
		virtual bool HasSampler() override { return textureSampler != nullptr; }

		inline virtual void* GetNativeHandle() override { return texture.Get(); }
		inline virtual void* GetImGuiHandle() { return GetSRV().Get(); }

		inline ID3D11Resource* GetID3D11Resource() { return texture.Get(); }

		inline ComPtr<ID3D11ShaderResourceView> GetSRV() { return views.srv; }
		inline ComPtr<ID3D11DepthStencilView> GetDSV() {return views.dsv;}
		inline ComPtr<ID3D11RenderTargetView> GetRTV() {return views.rtv;}
		inline ComPtr<ID3D11UnorderedAccessView> GetUAV() { return views.uav; };
		inline ComPtr<ID3D11SamplerState> GetSampler() { return textureSampler->GetSampler(); }
	protected:

	private:
		D3D11RenderDevice* device;
		D3D11Sampler* textureSampler = nullptr;
		D3D11ResourceViews views;
		ComPtr<ID3D11Texture2D> texture;
		
		

	};
}

