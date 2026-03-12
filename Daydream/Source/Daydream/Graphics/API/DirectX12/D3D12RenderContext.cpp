#include "DaydreamPCH.h"
#include "D3D12RenderContext.h"

namespace Daydream
{
	D3D12RenderContext::D3D12RenderContext(D3D12RenderDevice* _device, UInt32 _framesInFlight)
	{
		device = _device;

		commandLists.resize(_framesInFlight);
		commandAllocators.resize(_framesInFlight);
		for (UInt32 i = 0; i < _framesInFlight; i++)
		{
			HRESULT hr = device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocators[i].GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create command allocator {0}", i);

			hr = device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[i].Get(), nullptr, IID_PPV_ARGS(commandLists[i].GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create commandlist");

			commandLists[i]->Close();
		}
	}
	void D3D12RenderContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
	{
	}
	void D3D12RenderContext::SetClearColor(const Color& _color)
	{
	}

	void D3D12RenderContext::Clear()
	{
	}

	void D3D12RenderContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->GetCommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetCommandList()->DrawIndexedInstanced(_indexCount, 1, _startIndex, _baseVertex, 0);
		//device->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	}
	void D3D12RenderContext::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
	}
	void D3D12RenderContext::EndRenderPass(Shared<RenderPass> _renderPass)
	{
	}
	void D3D12RenderContext::BindPipelineState(Shared<PipelineState> _pipelineState)
	{
	}
	void D3D12RenderContext::BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
	}
	void D3D12RenderContext::BindIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
	}
	void D3D12RenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
	}
	void D3D12RenderContext::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
	}
	void D3D12RenderContext::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
	}
	void D3D12RenderContext::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
	}
	void D3D12RenderContext::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
	}
	void D3D12RenderContext::GenerateMips(Shared<Texture> _texture)
	{
	}
}
