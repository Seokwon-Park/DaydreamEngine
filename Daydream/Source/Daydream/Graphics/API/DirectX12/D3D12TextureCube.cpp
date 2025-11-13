#include "DaydreamPCH.h"
#include "D3D12TextureCube.h"

#include "D3D12Utility.h"
#include "D3D12Texture.h"

#include "Daydream/Graphics/Manager/ResourceManager.h"
#include "Daydream/Graphics/Resources/Mesh.h"
#include "D3D12PipelineState.h"

namespace Daydream
{
	D3D12TextureCube::D3D12TextureCube(D3D12RenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;

		mipLevels = _desc.mipLevels;
		format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format);

		textures.resize(6);

		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 6;
		textureDesc.MipLevels = mipLevels;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		texture = device->CreateTexture(textureDesc, D3D12_RESOURCE_STATE_COPY_DEST);
		SetCurrentState(D3D12_RESOURCE_STATE_COPY_DEST);

		//D3D12_SAMPLER_DESC samplerDesc = {};
		//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 선형 필터링
		//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 텍스처 주소 모드 (반복)
		//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//samplerDesc.MipLODBias = 0.0f;
		//samplerDesc.MaxAnisotropy = 1; // 비등방성 필터링 사용 안 함
		//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 비교 함수 (그림자 맵 등에서 사용)
		//samplerDesc.BorderColor[0] = 0.0f; // 경계 색상
		//samplerDesc.BorderColor[1] = 0.0f;
		//samplerDesc.BorderColor[2] = 0.0f;
		//samplerDesc.BorderColor[3] = 0.0f;
		//samplerDesc.MinLOD = 0.0f;
		//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX; // 모든 밉맵 레벨 사용

		//device->GetSamplerHeapAlloc().Alloc(&samplerCpuHandle, &samplerGpuHandle);

		//device->GetDevice()->CreateSampler(&samplerDesc, samplerCpuHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = mipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		device->GetCBVSRVUAVHeapAlloc().Alloc(&srvCpuHandle, &srvGpuHandle);
		device->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, srvCpuHandle);

		if (mipLevels > 1)
		{

			rtvCpuHandles.resize(mipLevels * 6);
			mipSrvCpuHandles.resize(mipLevels * 6);
			mipSrvGpuHandles.resize(mipLevels * 6);
			for (UInt32 mip = 0; mip < mipLevels; mip++)
			{
				for (UInt32 face = 0; face < 6; face++)
				{
					UInt32 index = mipLevels * face + mip;

					D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
					rtvDesc.Format = format;
					rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
					rtvDesc.Texture2DArray.MipSlice = mip;
					rtvDesc.Texture2DArray.FirstArraySlice = face;
					rtvDesc.Texture2DArray.ArraySize = 1;

					device->GetRTVHeapAlloc().Alloc(&rtvCpuHandles[index]);
					device->GetDevice()->CreateRenderTargetView(texture.Get(), &rtvDesc, rtvCpuHandles[index]);

					srvDesc.Format = format;
					srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

					// [핵심 1] 뷰 차원을 2D가 아닌 2D_ARRAY로 지정
					srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;

					// [핵심 2] 서브리소스를 1개의 밉, 1개의 면으로 한정
					srvDesc.Texture2DArray.MostDetailedMip = mip; // 원하는 밉 레벨
					srvDesc.Texture2DArray.MipLevels = 1;        // 1개만
					srvDesc.Texture2DArray.FirstArraySlice = face; // 원하는 큐브 면
					srvDesc.Texture2DArray.ArraySize = 1;        // 1개만
					srvDesc.Texture2DArray.PlaneSlice = 0;
					srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;

					device->GetCBVSRVUAVHeapAlloc().Alloc(&mipSrvCpuHandles[index], &mipSrvGpuHandles[index]);
					device->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, mipSrvCpuHandles[index]);
				}
			}

			resizeMaterial = Material::Create(ResourceManager::GetResource<PipelineState>("GenerateMipsPSO"));
		}

		//cubeFaceConstantBuffers.resize(6);
		//for (int i = 0; i < 6; i++)
		//{
		//	cubeFaceConstantBuffers[i] = ConstantBuffer::Create(sizeof(Matrix4x4));
		//	captureViewProjections.push_back(cubeFaceProjMatrix * cubeFaceViewMatrices[i]);
		//	captureViewProjections[i].MatrixTranspose();
		//	cubeFaceConstantBuffers[i]->Update(&captureViewProjections[i], sizeof(Matrix4x4));
		//}
	}
	D3D12TextureCube::~D3D12TextureCube()
	{
		resizeMaterial = nullptr;
		for (auto rtvHandle : rtvCpuHandles)
		{
			device->GetRTVHeapAlloc().Free(rtvHandle);
		}
		device->GetCBVSRVUAVHeapAlloc().Free(srvCpuHandle);

	}

	void D3D12TextureCube::SetSampler(Shared<Sampler> _sampler)
	{
		textureSampler = static_cast<D3D12Sampler*>(_sampler.get());
	}
	void D3D12TextureCube::GenerateMips()
	{
		//device->TransitionResourceState(device->GetCommandList(), texture.Get(), GetCurrentState(), D3D12_RESOURCE_STATE_RENDER_TARGET);
		//SetCurrentState(D3D12_RESOURCE_STATE_RENDER_TARGET);
		//float color[4] = { 1,1,1,1 };
		//for (UInt32 mip = 1; mip < mipLevels; mip++)
		//{
		//	for (UInt32 face = 0; face < 6; face++)
		//	{
		//		device->GetCommandList()->ClearRenderTargetView(rtvCpuHandles[mip*6+face], color, 0, nullptr);
		//	}
		//}
		for (UInt32 mip = 1; mip < mipLevels; mip++)
		{
			for (UInt32 face = 0; face < 6; face++)
			{
				UInt32 index = mipLevels * face + mip;

				//auto resizeRenderPass = ResourceManager::GetResource<RenderPass>("MipmapRenderPass");
				auto resizePSO = ResourceManager::GetResource<PipelineState>("GenerateMipsPSO");
				auto quadMesh = ResourceManager::GetResource<Mesh>("Quad");

				////TextureDesc textureDesc{};
				////textureDesc.width = fbDesc.width;
				////textureDesc.height = fbDesc.height;
				////textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
				////textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;c

				//
				//resizeMaterial->SetTexture2D("Texture", textures[face]);
				//resizeMaterial->SetConstantBuffer("Camera", cubeFaceConstantBuffers[face]);

				D3D12_RECT rect;
				rect.left = 0;
				rect.top = 0;
				rect.right = std::max(1, width >> mip);
				rect.bottom = std::max(1, height >> mip);

				device->GetCommandList()->RSSetScissorRects(1, &rect);

				D3D12_VIEWPORT viewport = {};
				viewport.Width = (Float32)std::max(1, width >> mip);
				viewport.Height = (Float32)std::max(1, height >> mip);
				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;

				device->GetCommandList()->RSSetViewports(1, &viewport);

				float color[4] = { 0,0,1,1 };

				device->TransitionResourceState(device->GetCommandList(), texture.Get(), GetCurrentState(), D3D12_RESOURCE_STATE_RENDER_TARGET);
				SetCurrentState(D3D12_RESOURCE_STATE_RENDER_TARGET);
				resizePSO->Bind();
				device->GetCommandList()->SetGraphicsRootDescriptorTable(1, mipSrvGpuHandles[mipLevels * face + mip - 1]);
				device->GetCommandList()->SetGraphicsRootDescriptorTable(0, textureSampler->GetSamplerHandle());
				quadMesh->Bind();
				device->GetCommandList()->ClearRenderTargetView(rtvCpuHandles[index], color, 0, nullptr);
				device->GetCommandList()->OMSetRenderTargets(1, &rtvCpuHandles[index], false, nullptr);

				device->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				device->GetCommandList()->DrawIndexedInstanced(quadMesh->GetIndexCount(), 1, 0, 0, 0);
				//resizeRenderPass->End();
				device->TransitionResourceState(device->GetCommandList(), texture.Get(), GetCurrentState(), D3D12_RESOURCE_STATE_COMMON);
				SetCurrentState(D3D12_RESOURCE_STATE_COMMON);
			}
		}
	}
}

