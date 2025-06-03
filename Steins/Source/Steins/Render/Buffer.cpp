#include "SteinsPCH.h"
#include "Buffer.h"
#include "GraphicsDevice.h"

#include "Steins/Render/Renderer.h"
#include "Steins/Render/GraphicsContext.h"

namespace Steins
{
	Shared<VertexBuffer> VertexBuffer::Create(UInt32 _bufferSize, UInt32 _stride)
	{
		return Renderer::GetRenderDevice()->CreateVertexBuffer(nullptr, _bufferSize, _stride);
	}
	Shared<VertexBuffer> VertexBuffer::Create(Float32* _vertices, UInt32 _size, UInt32 _stride)
	{
		return Renderer::GetRenderDevice()->CreateVertexBuffer(_vertices, _size, _stride);
	}
	Shared<IndexBuffer> IndexBuffer::Create(UInt32* _indices, UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateIndexBuffer(_indices, _size);
	}
	Shared<ConstantBuffer> ConstantBuffer::Create(UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateConstantBuffer(_size);
	}
}
