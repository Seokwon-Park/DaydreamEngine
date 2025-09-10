#include "DaydreamPCH.h"
#include "D3D11TextureCube.h"

#include "D3D11Sampler.h"

#include "Daydream/Graphics/API/DirectX/D3DUtility.h"

namespace Daydream
{
	//namespace
	//{
	//	bool CreateDebugCubemap(ID3D11Device* device, ID3D11Texture2D** texture, ID3D11ShaderResourceView** srv) {
	//		const UINT size = 256;
	//		const UINT faces = 6;

	//		// 각 면의 색상
	//		UINT colors[6] = {
	//			0xFF0000FF, // +X: Red
	//			0xFF00FF00, // -X: Green  
	//			0xFFFF0000, // +Y: Blue
	//			0xFF00FFFF, // -Y: Cyan
	//			0xFFFF00FF, // +Z: Magenta
	//			0xFFFFFF00  // -Z: Yellow
	//		};

	//		std::vector<std::vector<UINT>> faceData(faces);
	//		for (int face = 0; face < faces; ++face) {
	//			faceData[face].resize(size * size, colors[face]);
	//		}

	//		D3D11_TEXTURE2D_DESC texDesc = {};
	//		texDesc.Width = size;
	//		texDesc.Height = size;
	//		texDesc.MipLevels = 1;
	//		texDesc.ArraySize = faces;
	//		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//		texDesc.SampleDesc.Count = 1;
	//		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//		texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//		std::vector<D3D11_SUBRESOURCE_DATA> subResources(faces);
	//		for (int face = 0; face < faces; ++face) {
	//			subResources[face].pSysMem = faceData[face].data();
	//			subResources[face].SysMemPitch = size * 4;
	//			subResources[face].SysMemSlicePitch = 0;
	//		}

	//		HRESULT hr = device->CreateTexture2D(&texDesc, subResources.data(), texture);
	//		if (FAILED(hr)) return false;

	//		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//		srvDesc.Format = texDesc.Format;
	//		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//		srvDesc.TextureCube.MipLevels = 1;

	//		hr = device->CreateShaderResourceView(*texture, &srvDesc, srv);
	//		return SUCCEEDED(hr);
	//	}
	//}
	D3D11TextureCube::D3D11TextureCube(D3D11RenderDevice* _device, const TextureDesc& _desc, Array<Array<UInt8>> _initialData)
	{
		device = _device;
		desc = _desc;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = desc.width;
		textureDesc.Height = desc.height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = _initialData.size(); // 큐브맵은 6개의 텍스처 배열입니다.
		textureDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format); // DXGI_FORMAT 열거형 값
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // 일반적인 텍스처 사용법
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SUBRESOURCE_DATA subresourceData[6];
		bool hasInitialData = (_initialData.size() == 6);

		for (int i = 0; i < 6; i++)
		{
			subresourceData[i].pSysMem = _initialData[i].data();
			subresourceData[i].SysMemPitch = textureDesc.Width * sizeof(UInt8) * 4;
			subresourceData[i].SysMemSlicePitch = 0; 
		}

		HRESULT hr = device->GetDevice()->CreateTexture2D(
			&textureDesc,
			&subresourceData[0],
			texture.GetAddressOf() 
		);

		//CreateDebugCubemap(device->GetDevice(), texture.GetAddressOf(), views.srv.GetAddressOf());

	if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = 1;
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
		textureSampler = sampler->GetSampler();
	}
}