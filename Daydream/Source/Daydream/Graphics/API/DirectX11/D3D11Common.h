#pragma once

namespace Daydream
{
	struct D3D11ResourceViews
	{
		ComPtr<ID3D11ShaderResourceView> srv;
		ComPtr<ID3D11DepthStencilView> dsv;
		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11UnorderedAccessView> uav;
	};
}