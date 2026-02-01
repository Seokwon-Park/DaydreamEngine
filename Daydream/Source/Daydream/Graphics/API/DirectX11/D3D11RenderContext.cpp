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
	D3D11GraphicsContext::D3D11GraphicsContext(D3D11RenderDevice* _device)
	{
		device = _device;
	}

	void D3D11GraphicsContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
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
	void D3D11GraphicsContext::SetClearColor(const Color& _color)
	{
		clearColor = _color;
	}

	void D3D11GraphicsContext::Clear()
	{
	}

	void D3D11GraphicsContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetContext()->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
	void D3D11GraphicsContext::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
		Shared<D3D11Framebuffer> currentFramebuffer = static_pointer_cast<D3D11Framebuffer>(_framebuffer);
		Array<ID3D11RenderTargetView*> rtvs = currentFramebuffer->GetRenderTargetViews();
		for (auto rtv : rtvs)
		{
			device->GetContext()->ClearRenderTargetView(rtv, _renderPass->GetClearColor().color);
		}
		if (currentFramebuffer->HasDepthAttachment())
		{
			device->GetContext()->ClearDepthStencilView(currentFramebuffer->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			device->GetContext()->OMSetRenderTargets((UInt32)rtvs.size(), rtvs.data(), currentFramebuffer->GetDepthStencilView().Get());
		}
		else
		{
			device->GetContext()->OMSetRenderTargets((UInt32)rtvs.size(), rtvs.data(), nullptr);
		}

	}
	void D3D11GraphicsContext::EndRenderPass(Shared<RenderPass> _renderPass)
	{
		device->GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
	}
	void D3D11GraphicsContext::BindPipelineState(Shared<PipelineState> _pipelineState)
	{
		_pipelineState->Bind();
	}
	void D3D11GraphicsContext::BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		UInt32 offset = 0;
		UInt32 stride = _vertexBuffer->GetStride();
		ID3D11Buffer* vertexBuffer = (ID3D11Buffer*)_vertexBuffer->GetNativeHandle();
		device->GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	}
	void D3D11GraphicsContext::BindIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		UInt32 offset = 0;
		ID3D11Buffer* indexBuffer = (ID3D11Buffer*)_indexBuffer->GetNativeHandle();
		device->GetContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	}
	void D3D11GraphicsContext::BindMesh(Shared<Mesh> _mesh)
	{
		BindVertexBuffer(_mesh->GetVertexBuffer());
		BindIndexBuffer(_mesh->GetIndexBuffer());
	}
	void D3D11GraphicsContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
	}
	void D3D11GraphicsContext::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
	}
	void D3D11GraphicsContext::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
	}
	void D3D11GraphicsContext::SetMaterial(Shared<Material> _material)
	{
	}
	void D3D11GraphicsContext::Submit()
	{
	}
	void D3D11GraphicsContext::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
	}
	void D3D11GraphicsContext::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
	}
	void D3D11GraphicsContext::GenerateMips(Shared<Texture> _texture)
	{
		device->GetContext()->GenerateMips((ID3D11ShaderResourceView*)_texture->GetNativeHandle());
	}
}
