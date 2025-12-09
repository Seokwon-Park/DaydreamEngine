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
		virtual void GenerateMips() override;


		inline ID3D11Resource* GetID3D11Resource() { return texture.Get(); }

		inline ComPtr<ID3D11ShaderResourceView> GetSRV() { return views.srv; }
		inline ComPtr<ID3D11DepthStencilView> GetDSV() { return views.dsv; }
		inline ComPtr<ID3D11RenderTargetView> GetRTV() { return views.rtv; }
		inline ComPtr<ID3D11UnorderedAccessView> GetUAV() { return views.uav; };
		inline ComPtr<ID3D11SamplerState> GetSampler() { return textureSampler; }

	protected:
		D3D11RenderDevice* device;
		D3D11ResourceViews views;

		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11SamplerState> textureSampler;

	};
}