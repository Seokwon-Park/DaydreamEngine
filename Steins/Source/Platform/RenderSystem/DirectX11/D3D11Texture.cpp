#include "SteinsPCH.h"
#include "D3D11Texture.h"

#include "stb_image.h"

namespace Steins
{
	D3D11Texture2D::D3D11Texture2D(D3D11GraphicsDevice* _device, const FilePath& _path)
		:Texture2D(_path)
	{
		device = _device;

		data = stbi_load(_path.ToString().c_str(), &width, &height, &channels, 0);
		STEINS_CORE_ASSERT(data, "Failed to load image!");

		UInt8* newPixels = new UInt8[width * height*4];
		if (channels == 3)
		{
			for (int i = 0; i < width * height; i++)
			{
				newPixels[i*4] = data[i*3];
				newPixels[i*4+1] = data[i*3+1];
				newPixels[i*4+2] = data[i*3+2];
				newPixels[i*4+3] = 255;
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

		D3D11_SUBRESOURCE_DATA pixelData{};
		pixelData.pSysMem = newPixels;
		pixelData.SysMemPitch = desc.Width * sizeof(UInt8) * 4; //RGBA

		device->GetDevice()->CreateTexture2D(&desc, &pixelData, texture.GetAddressOf());
		device->GetDevice()->CreateShaderResourceView(texture.Get(), nullptr, textureSrv.GetAddressOf());
	}
	D3D11Texture2D::~D3D11Texture2D()
	{
	}
	void D3D11Texture2D::Bind(UInt32 _slot) const
	{
		device->GetContext()->PSSetShaderResources(_slot, 1, textureSrv.GetAddressOf());
	}
}
