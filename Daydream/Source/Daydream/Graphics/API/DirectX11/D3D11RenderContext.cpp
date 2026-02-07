#include "DaydreamPCH.h"
#include "D3D11RenderContext.h"
#include "D3D11RenderDevice.h"

#include "D3D11PipelineState.h"
#include "D3D11Texture.h"
#include "D3D11TextureCube.h"
#include "D3D11Buffer.h"
#include "D3D11Framebuffer.h"

#include "Daydream/Graphics/Resources/Mesh.h"

namespace Daydream
{
	D3D11RenderContext::D3D11RenderContext(D3D11RenderDevice* _device)
	{
		device = _device;
	}

	void D3D11RenderContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
	{
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = Cast<Float32>(_x);
		viewport.TopLeftY = Cast<Float32>(_y);
		viewport.Width = Cast<Float32>(_width);
		viewport.Height = Cast<Float32>(_height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		device->GetContext()->RSSetViewports(1, &viewport);
	}
	void D3D11RenderContext::SetClearColor(const Color& _color)
	{
		clearColor = _color;
	}

	void D3D11RenderContext::Clear()
	{
	}

	void D3D11RenderContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->GetContext()->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
	void D3D11RenderContext::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
		D3D11Framebuffer* currentFramebuffer = Cast<D3D11Framebuffer*>(_framebuffer.get());
		Array<ID3D11RenderTargetView*> rtvs = currentFramebuffer->GetRenderTargetViews();
		for (auto rtv : rtvs)
		{
			device->GetContext()->ClearRenderTargetView(rtv, _renderPass->GetClearColor().color);
		}
		if (currentFramebuffer->HasDepthAttachment())
		{
			device->GetContext()->ClearDepthStencilView(currentFramebuffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			device->GetContext()->OMSetRenderTargets((UInt32)rtvs.size(), rtvs.data(), currentFramebuffer->GetDepthStencilView());
		}
		else
		{
			device->GetContext()->OMSetRenderTargets((UInt32)rtvs.size(), rtvs.data(), nullptr);
		}

		SetViewport(0, 0, _framebuffer->GetWidth(), _framebuffer->GetHeight());
	}
	void D3D11RenderContext::EndRenderPass(Shared<RenderPass> _renderPass)
	{
		device->GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
		device->GetContext()->PSSetShaderResources(0, 20, nullSRVs.data());
	}
	void D3D11RenderContext::BindPipelineState(Shared<PipelineState> _pipelineState)
	{
		RenderContext::BindPipelineState(_pipelineState);
		D3D11PipelineState* pso = Cast<D3D11PipelineState*>(currentPipelineState.get());
		pso->BindPipelineState();
	}
	void D3D11RenderContext::BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		UInt32 offset = 0;
		UInt32 stride = _vertexBuffer->GetStride();

		ID3D11Buffer* vertexBuffer = Cast<D3D11VertexBuffer*>(_vertexBuffer.get())->GetID3D11Buffer();
		device->GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	}
	void D3D11RenderContext::BindIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		UInt32 offset = 0;
		ID3D11Buffer* indexBuffer = Cast<D3D11IndexBuffer*>(_indexBuffer.get())->GetID3D11Buffer();
		device->GetContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	}

	void D3D11RenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		RenderContext::SetTexture2D(_name, _texture);
		const ShaderReflectionData* resourceInfo = currentPipelineState->GetBindingInfo(_name);
		if (resourceInfo == nullptr) return;

		//	DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");

		Shared<D3D11Texture2D> d3d11Texture = static_pointer_cast<D3D11Texture2D>(_texture);
		ID3D11ShaderResourceView* srv = d3d11Texture->GetSRV();
		ID3D11SamplerState* sampler = d3d11Texture->GetSampler();
		switch (resourceInfo->shaderType)
		{
		case Daydream::ShaderType::None:
			DAYDREAM_CORE_ASSERT(false, "ERROR");
			break;
		case Daydream::ShaderType::Vertex:
		{
			device->GetContext()->VSSetShaderResources(resourceInfo->binding, 1, &srv);
			device->GetContext()->VSSetSamplers(resourceInfo->binding, 1, &sampler);
			break;
		}
		case Daydream::ShaderType::Hull:
			break;
		case Daydream::ShaderType::Domain:
			break;
		case Daydream::ShaderType::Geometry:
			break;
		case Daydream::ShaderType::Pixel:
		{
			device->GetContext()->PSSetShaderResources(resourceInfo->binding, 1, &srv);
			device->GetContext()->PSSetSamplers(resourceInfo->binding, 1, &sampler);
			break;
		}
		case Daydream::ShaderType::Compute:
			break;
		default:
			break;
		}
	}

	void D3D11RenderContext::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		const ShaderReflectionData* bindingInfo = currentPipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;
		//DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");
		Shared<D3D11TextureCube> d3d11Texture = static_pointer_cast<D3D11TextureCube>(_textureCube);
		ID3D11ShaderResourceView* srv = d3d11Texture->GetSRV();
		ID3D11SamplerState* sampler = d3d11Texture->GetSampler();
		switch (bindingInfo->shaderType)
		{
		case Daydream::ShaderType::None:
			DAYDREAM_CORE_ASSERT(false, "ERROR");
			break;
		case Daydream::ShaderType::Vertex:
		{
			device->GetContext()->VSSetShaderResources(bindingInfo->binding, 1, &srv);
			device->GetContext()->VSSetSamplers(bindingInfo->binding, 1, &sampler);
			break;
		}
		case Daydream::ShaderType::Hull:
			break;
		case Daydream::ShaderType::Domain:
			break;
		case Daydream::ShaderType::Geometry:
			break;
		case Daydream::ShaderType::Pixel:
		{
			device->GetContext()->PSSetShaderResources(bindingInfo->binding, 1, &srv);
			device->GetContext()->PSSetSamplers(bindingInfo->binding, 1, &sampler);
			break;
		}
		case Daydream::ShaderType::Compute:
			break;
		default:
			break;
		}
	}
	void D3D11RenderContext::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		const ShaderReflectionData* resourceInfo = currentPipelineState->GetBindingInfo(_name);
		if (resourceInfo == nullptr) return;
		DAYDREAM_CORE_ASSERT(device->GetAPI() == RendererAPIType::DirectX11, "Wrong API!");
		Shared<D3D11ConstantBuffer> d3d11Buffer = static_pointer_cast<D3D11ConstantBuffer>(_buffer);
		switch (resourceInfo->shaderType)
		{
		case ShaderType::None:
			break;
		case ShaderType::Vertex:
			device->GetContext()->VSSetConstantBuffers(resourceInfo->binding, 1, d3d11Buffer->GetBuffer().GetAddressOf());
			break;
		case ShaderType::Hull:
			break;
		case ShaderType::Domain:
			break;
		case ShaderType::Geometry:
			break;
		case ShaderType::Pixel:
			device->GetContext()->PSSetConstantBuffers(resourceInfo->binding, 1, d3d11Buffer->GetBuffer().GetAddressOf());
			break;
		case ShaderType::Compute:
			break;
		default:
			break;
		}
	}
	
	void D3D11RenderContext::Submit()
	{
	}
	void D3D11RenderContext::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
		device->GetContext()->CopyResource((ID3D11Resource*)_dst->GetNativeHandle(), (ID3D11Resource*)_src->GetNativeHandle());
	}
	void D3D11RenderContext::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
		D3D11TextureCube* dst = Cast<D3D11TextureCube*>(_dstCubemap.get());
		D3D11Texture2D* src = Cast<D3D11Texture2D*>(_srcTexture2D.get());
		// 큐브맵의 MipLevels 정보를 가져와야 Subresource 인덱스를 계산할 수 있습니다.
		D3D11_TEXTURE2D_DESC cubeDesc;
		dst->GetID3D11Texture2D()->GetDesc(&cubeDesc);

		// 1. 핵심: 대상 Subresource 인덱스를 계산합니다.
		// 밉맵 레벨 0, 배열 슬라이스 faceIndex에 해당하는 Subresource를 찾습니다.
		UINT dstSubresourceIndex = D3D11CalcSubresource(
			_mipLevel,
			_faceIndex,
			cubeDesc.MipLevels
		);

		// 2. 원본 Subresource 인덱스는 항상 0입니다 (2D 텍스처는 Subresource가 하나).
		UINT srcSubresourceIndex = 0;

		// 3. 복사 명령을 즉시 실행합니다.
		// DX11에서는 별도의 리소스 배리어가 필요 없습니다.
		device->GetContext()->CopySubresourceRegion(
			dst->GetID3D11Texture2D(),           // 대상 리소스
			dstSubresourceIndex,  // 대상 Subresource
			0, 0, 0,              // 대상 좌표 (x, y, z)
			src->GetID3D11Texture2D(),         // 원본 리소스
			srcSubresourceIndex,  // 원본 Subresource
			nullptr               // 원본 영역 (nullptr은 전체를 의미)
		);
	}
	void D3D11RenderContext::GenerateMips(Shared<Texture> _texture)
	{
		ID3D11ShaderResourceView* srv = nullptr;
		switch (_texture->GetType())
		{
		case TextureType::Texture2D:
			srv = Cast<D3D11Texture2D*>(_texture.get())->GetSRV();
			break;
		case TextureType::TextureCube:
			srv = Cast<D3D11TextureCube*>(_texture.get())->GetSRV();
			break;
		}

		if (srv)
		{
			device->GetContext()->GenerateMips(srv);
		}
	}
}
