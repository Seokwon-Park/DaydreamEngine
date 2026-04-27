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

	Shared<VertexBuffer> VertexBuffer::CreateDynamic(UInt32 _size, UInt32 _stride, const void* _initialData, UInt32 _initialDataSize)
	{
		BufferDesc desc{};
		desc.bufferUsage = BufferUsage::Vertex;
		desc.memoryUsage = MemoryUsage::Dynamic;
		desc.size = _size;

		Shared<GPUBuffer> gpuBuffer = Renderer::GetRenderDevice()->CreateGPUBuffer(desc);
		Shared<VertexBuffer> vertexBuffer = MakeShared<VertexBuffer>(gpuBuffer, _stride);
		
		if (_initialData != nullptr && _initialDataSize > 0)
			vertexBuffer->UpdateData(_initialData, _initialDataSize);

		return vertexBuffer;
	}
	Shared<VertexBuffer> VertexBuffer::CreateStatic(UInt32 _size, UInt32 _stride, const void* _initialData)
	{
		DAYDREAM_CORE_ASSERT(_initialData, "Static Vertex Buffer must have initial data!");

		BufferDesc desc{};
		desc.bufferUsage = BufferUsage::Vertex;
		desc.memoryUsage = MemoryUsage::Static;
		desc.size = _size;

		Shared<GPUBuffer> gpuBuffer = Renderer::GetRenderDevice()->CreateGPUBuffer(desc);
		Shared<VertexBuffer> vertexBuffer = MakeShared<VertexBuffer>(gpuBuffer, _stride);
		Shared<UploadBuffer> uploadBuffer = UploadBuffer::Create(_size);
		uploadBuffer->UpdateData(_initialData, _size);

		Renderer::CopyBuffer

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

	UploadBuffer::UploadBuffer(Shared<GPUBuffer> _buffer)
		:buffer(_buffer)
	{

	}

	Shared<UploadBuffer> UploadBuffer::Create(UInt32 _size)
	{
		BufferDesc desc{};
		desc.bufferUsage = BufferUsage::TransferDest;
		desc.memoryUsage = MemoryUsage::Dynamic;
		desc.size = _size;

		Shared<GPUBuffer> gpuBuffer = Renderer::GetRenderDevice()->CreateGPUBuffer(desc);
		return MakeShared<UploadBuffer>(gpuBuffer);
	}
}

