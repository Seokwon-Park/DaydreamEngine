#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Core/RenderDevice.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Core/RenderContext.h"


namespace Daydream
{
	GPUBuffer::GPUBuffer(const BufferDesc& _desc)
	{
		desc = _desc;
	}

	VertexBuffer::VertexBuffer(Shared<GPUBuffer> _buffer, UInt32 _stride)
		:buffer(_buffer), stride(_stride)
	{

	}

	Shared<VertexBuffer> VertexBuffer::CreateDynamic(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize, const void* _initialData)
	{
		return Renderer::GetRenderDevice()->CreateDynamicVertexBuffer(_size, _stride, 0, _initialData);
	}
	Shared<VertexBuffer> VertexBuffer::CreateStatic(UInt32 _size, UInt32 _stride, const void* _initialData)
	{
		DAYDREAM_CORE_ASSERT(_initialData, "Static Vertex Buffer must have initial data!");
		return Renderer::GetRenderDevice()->CreateStaticVertexBuffer(_size, _stride, _initialData);
	}

	IndexBuffer::IndexBuffer(Shared<GPUBuffer> _buffer, UInt32 _indexCount)
		:buffer(_buffer), indexCount(_indexCount)
	{
	}

	Shared<IndexBuffer> IndexBuffer::Create(const UInt32* _indices, UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateIndexBuffer(_indices, _size);
	}

	ConstantBuffer::ConstantBuffer(Shared<GPUBuffer> _buffer)
		:buffer(_buffer)
	{
	}

	Shared<ConstantBuffer> ConstantBuffer::Create(UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateConstantBuffer(_size);
	}
}

