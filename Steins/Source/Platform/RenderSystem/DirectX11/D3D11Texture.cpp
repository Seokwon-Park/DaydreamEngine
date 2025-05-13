#include "SteinsPCH.h"
#include "D3D11Texture.h"

#include "stb_image.h"

namespace Steins
{
	D3D11Texture2D::D3D11Texture2D(D3D11GraphicsDevice* _device, const FilePath& _path)
		:Texture2D(_path)
	{
		device = _device;

		UInt8* newPixels = new UInt8[width * height];
		if (channels == 3)
		{
			for (int i = 0; i < width * height; i++)
			{
				newPixels[i] = data[i];
				newPixels[i+1] = data[i+1];
				newPixels[i+2] = data[i+2];
				newPixels[i+3] = 255;
			}
		}
		else if (channels == 4)
		{
			memcpy(newPixels, data, width * height * 4 * sizeof(UInt8));
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

		D3D11_SUBRESOURCE_DATA data{};
		data.SysMemPitch = desc.Width * sizeof(UInt8) * 4; //RGBA
		data.pSysMem = newPixels;

		device->GetDevice()->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());
	}
	D3D11Texture2D::~D3D11Texture2D()
	{
	}
	void D3D11Texture2D::Bind(UInt32 _slot) const
	{
	}
}
