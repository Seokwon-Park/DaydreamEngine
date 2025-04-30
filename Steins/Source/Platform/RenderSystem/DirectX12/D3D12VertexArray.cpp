#include "SteinsPCH.h"
#include "D3D12VertexArray.h"

namespace Steins
{
	D3D12VertexArray::D3D12VertexArray(D3D12GraphicsDevice* _device)
	{
		device = _device;
	}
	D3D12VertexArray::~D3D12VertexArray()
	{
	}
	void D3D12VertexArray::Bind() const
	{
		for (auto vertexBuffer : vertexBuffers)
		{
			vertexBuffer->Bind();
		}
		indexBuffer->Bind();
	}
	void D3D12VertexArray::Unbind() const
	{
	}
	void D3D12VertexArray::AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer)
	{
		_vertexBuffer->SetSlot((UInt32)vertexBuffers.size());
		vertexBuffers.push_back(_vertexBuffer);
	}
	void D3D12VertexArray::SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer)
	{
		indexBuffer = _indexBuffer;
	}
}