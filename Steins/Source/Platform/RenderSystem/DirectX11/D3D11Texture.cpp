#include "SteinsPCH.h"
#include "D3D11Texture.h"

#include "stb_image.h"

namespace Steins
{
	D3D11Texture2D::D3D11Texture2D(D3D11GraphicsDevice* _device, const FilePath& _path)
		:Texture2D(_path)
	{
		device = _device;

		DXGI_FORMAT dataFormat;
		if (channels == 4)
		{
			dataFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if (channels == 3)
		{
			dataFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1; 
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;


		device->GetDevice()->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
	}
	D3D11Texture2D::~D3D11Texture2D()
	{
	}
	void D3D11Texture2D::Bind(UInt32 _slot) const
	{
	}
}
