#include "SteinsPCH.h"
#include "D3D11Texture.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"
#include "Steins/Graphics/Utility/ImageLoader.h"

namespace Steins
{
	D3D11Texture2D::D3D11Texture2D(D3D11RenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = GraphicsUtil::RenderFormatToDXGIFormat(_desc.format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = GraphicsUtil::ConvertToD3D11BindFlags(_desc.bindFlags);
		textureDesc.CPUAccessFlags = 0;

		HRESULT hr = device->GetDevice()->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Texture!");
	}
	D3D11Texture2D::D3D11Texture2D(D3D11RenderDevice* _device, const FilePath& _path, const TextureDesc& _desc)
		:Texture2D(_path)
	{
		device = _device;

		Array<UInt8> imageData = ImageLoader::LoadImageFile(_path, width, height, channels);

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = GraphicsUtil::RenderFormatToDXGIFormat(_desc.format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = GraphicsUtil::ConvertToD3D11BindFlags(_desc.bindFlags);
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA pixelData{};
		pixelData.pSysMem = imageData.data();
		pixelData.SysMemPitch = textureDesc.Width * sizeof(UInt8) * 4; //RGBA

		device->GetDevice()->CreateTexture2D(&textureDesc, &pixelData, texture.GetAddressOf());
	}

	D3D11Texture2D::D3D11Texture2D(D3D11RenderDevice* _device, ComPtr<ID3D11Texture2D> _texture)
	{
		device = _device;
		texture = _texture;
	}
	
	D3D11Texture2D::~D3D11Texture2D()
	{
	}
	ComPtr<ID3D11ShaderResourceView> D3D11Texture2D::GetSRV()
	{
		if (srv != nullptr) return srv;
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);
		if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			device->GetDevice()->CreateShaderResourceView(texture.Get(), nullptr, srv.GetAddressOf());
			return srv;
		}
		STEINS_CORE_ASSERT(srv, "This texture was not created with the Shader Resource View (SRV) bind flag.");
		return nullptr;
	}
	ComPtr<ID3D11DepthStencilView> D3D11Texture2D::GetDSV()
	{
		if (dsv != nullptr) return dsv;
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);
		if (textureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			device->GetDevice()->CreateDepthStencilView(texture.Get(), nullptr, dsv.GetAddressOf());
			return dsv;
		}
		STEINS_CORE_ASSERT(dsv, "This texture was not created with the Depth Stencil View (DSV) bind flag.");
		return nullptr;
	}
	ComPtr<ID3D11RenderTargetView> D3D11Texture2D::GetRTV()
	{
		if (rtv != nullptr) return rtv;
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);
		if (textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			device->GetDevice()->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf());
			return rtv;
		}
		STEINS_CORE_ASSERT(rtv, "This texture was not created with the Render Target View (RTV) bind flag.");
		return nullptr;
	}
	ComPtr<ID3D11UnorderedAccessView> D3D11Texture2D::GetUAV()
	{
		if (uav != nullptr) return uav;
		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);
		if (textureDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			device->GetDevice()->CreateUnorderedAccessView(texture.Get(), nullptr, uav.GetAddressOf());
			return uav;
		}
		STEINS_CORE_ASSERT(uav, "This texture was not created with the Unordered Access View (UAV) bind flag.");
		return nullptr;
	}
}
