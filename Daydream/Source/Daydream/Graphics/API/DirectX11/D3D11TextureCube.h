#pragma once

#pragma once

#include "D3D11Texture.h"
#include "D3D11RenderDevice.h"
#include "D3D11Common.h"
#include "Daydream/Graphics/Resources/TextureCube.h"


namespace Daydream
{
	class D3D11TextureCube : public TextureCube
	{
	public:
		D3D11TextureCube(D3D11RenderDevice* _device, const TextureDesc& _desc, Array<const void*>& _initialData);

		virtual ~D3D11TextureCube();

		virtual void* GetNativeHandle() { return texture.Get(); };

		virtual void SetSampler(Shared<Sampler> _sampler) override;
		virtual bool HasSampler() override { return textureSampler != nullptr; }

		inline ID3D11Texture2D* GetID3D11Texture2D() { return texture.Get(); }

		inline ID3D11ShaderResourceView* GetSRV() { return views.srv.Get(); }
		inline ID3D11DepthStencilView* GetDSV() { return views.dsv.Get(); }
		inline ID3D11RenderTargetView* GetRTV() { return views.rtv.Get(); }
		inline ID3D11UnorderedAccessView* GetUAV() { return views.uav.Get(); };
		inline ID3D11SamplerState* GetSampler() { return textureSampler->GetSampler(); }

	protected:
		D3D11RenderDevice* device;
		D3D11ResourceViews views;

		ComPtr<ID3D11Texture2D> texture;
		D3D11Sampler* textureSampler = nullptr;

	};
}