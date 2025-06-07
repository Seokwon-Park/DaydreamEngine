#include "SteinsPCH.h"
#include "Buffer.h"
#include "RenderDevice.h"

#include "Steins/Render/Renderer.h"
#include "Steins/Render/GraphicsContext.h"

namespace Steins
{
	Shared<VertexBuffer> VertexBuffer::CreateDynamic(UInt32 _bufferSize, UInt32 _stride)
	{
		return Renderer::GetRenderDevice()->CreateDynamicVertexBuffer(_bufferSize, _stride);
	}
	Shared<VertexBuffer> VertexBuffer::CreateStatic(Float32* _vertices, UInt32 _size, UInt32 _stride)
	{
		return Renderer::GetRenderDevice()->CreateStaticVertexBuffer(_vertices, _size, _stride);
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
