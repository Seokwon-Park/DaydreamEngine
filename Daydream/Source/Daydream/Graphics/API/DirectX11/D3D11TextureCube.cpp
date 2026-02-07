#include "DaydreamPCH.h"
#include "D3D11TextureCube.h"

#include "D3D11Sampler.h"

#include "Daydream/Graphics/API/DirectX/D3DUtility.h"

namespace Daydream
{
	D3D11TextureCube::D3D11TextureCube(D3D11RenderDevice* _device, const TextureDesc& _desc, Array<const void*>& _initialData)
		:TextureCube(_desc)
	{
		device = _device;


		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = desc.width;
		textureDesc.Height = desc.height;
		textureDesc.MipLevels = _desc.mipLevels;
		textureDesc.ArraySize = 6; // 큐브맵은 6개의 텍스처 배열입니다.
		textureDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format); // DXGI_FORMAT 열거형 값
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // 일반적인 텍스처 사용법
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA subresourceData[6];
		bool hasInitialData = (_initialData.size() == 6);

		if (hasInitialData)
		{
			for (int i = 0; i < 6; i++)
			{
				subresourceData[i].pSysMem = _initialData[i];
				subresourceData[i].SysMemPitch = textureDesc.Width * sizeof(UInt8) * 4;
				subresourceData[i].SysMemSlicePitch = 0;
			}

			HRESULT hr = device->GetDevice()->CreateTexture2D(
				&textureDesc,
				&subresourceData[0],
				texture.GetAddressOf()
			);
		}
		else
		{
			HRESULT hr = device->GetDevice()->CreateTexture2D(
				&textureDesc,
				nullptr,
				texture.GetAddressOf()
			);
		}

		

		//CreateDebugCubemap(device->GetDevice(), texture.GetAddressOf(), views.srv.GetAddressOf());

		if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = -1;
			device->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, views.srv.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.srv, "This texture was not created with the Shader Resource View (SRV) bind flag.");
		}

		if (textureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			device->GetDevice()->CreateDepthStencilView(texture.Get(), nullptr, views.dsv.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.dsv, "This texture was not created with the Depth Stencil View (DSV) bind flag.");
		}

		if (textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			device->GetDevice()->CreateRenderTargetView(texture.Get(), nullptr, views.rtv.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.rtv, "This texture was not created with the Render Target View (RTV) bind flag.");

		}

		if (textureDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			device->GetDevice()->CreateUnorderedAccessView(texture.Get(), nullptr, views.uav.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.uav, "This texture was not created with the Unordered Access View (UAV) bind flag.");
		}
	}

	D3D11TextureCube::~D3D11TextureCube()
	{
	}

	void D3D11TextureCube::SetSampler(Shared<Sampler> _sampler)
	{
		auto sampler = static_pointer_cast<D3D11Sampler>(_sampler);
		textureSampler = sampler.get();
	}
}