#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Core/RenderDevice.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Core/RenderContext.h"

namespace Daydream
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
