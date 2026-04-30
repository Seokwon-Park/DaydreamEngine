#pragma once

#include "Daydream/Graphics/Resources/TextureView.h"
#include "D3D11Common.h"

namespace Daydream
{
	class D3D11TextureView : public TextureView
	{
	public:
		explicit D3D11TextureView(const TextureViewDesc& _desc);
		~D3D11TextureView() override = default;

		const TextureViewDesc& GetDesc() const override { return desc; }

		ID3D11ShaderResourceView* GetSRV() const { return views.srv.Get(); }
		ID3D11DepthStencilView* GetDSV() const { return views.dsv.Get(); }
		ID3D11RenderTargetView* GetRTV() const { return views.rtv.Get(); }
		ID3D11UnorderedAccessView* GetUAV() const { return views.uav.Get(); }

		D3D11ResourceViews& GetViews() { return views; }
	protected:

	private:

	private:
		TextureViewDesc desc;
		D3D11ResourceViews views;
	};
}
