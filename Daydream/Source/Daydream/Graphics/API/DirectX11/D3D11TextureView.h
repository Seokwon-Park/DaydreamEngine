#pragma once

#include "D3D11Common.h"
#include "D3D11RenderDevice.h"
#include "D3D11Texture.h"
#include "Daydream/Graphics/Resources/Texture/TextureView.h"

namespace Daydream
{
	class D3D11TextureView : public TextureView
	{
	public:
		D3D11TextureView(D3D11RenderDevice* _device, Shared<D3D11GPUTexture> _texture, const TextureViewDesc& _desc);
		~D3D11TextureView() override = default;

		virtual void* GetUIHandle() const { return views.srv.Get(); }

		ID3D11ShaderResourceView* GetSRV() const { return views.srv.Get(); }
		ID3D11DepthStencilView* GetDSV() const { return views.dsv.Get(); }
		ID3D11RenderTargetView* GetRTV() const { return views.rtv.Get(); }
		ID3D11UnorderedAccessView* GetUAV() const { return views.uav.Get(); }

		D3D11ResourceViews& GetViews() { return views; }
	protected:

	private:

	private:
		ComPtr<ID3D11View> view;
		D3D11ResourceViews views;
	};
}
