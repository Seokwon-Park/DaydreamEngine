#pragma once

#include "Daydream/Enum/RendererEnums.h"

namespace Daydream
{
	struct BufferDesc
	{
		UInt32 size = 0;
		BufferUsage bufferUsage = BufferUsage::None;
		MemoryUsage memoryUsage = MemoryUsage::Static;
		const void* initialData = nullptr;
	};

	class GPUBuffer
	{
	public:
		GPUBuffer(const BufferDesc& _desc);
		virtual ~GPUBuffer() {}

		virtual void UpdateData(const void* _data, UInt32 _size) = 0;

		inline UInt32 GetSize() const { return desc.size; }

		const BufferDesc& GetDesc() const { return desc; }

		/*static Shared<GPUBuffer> Create(const BufferDesc& _desc);*/
	protected:
		BufferDesc desc;
		void* mappedData;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(Shared<GPUBuffer> _buffer, UInt32 _stride);
		~VertexBuffer() {}

		inline void UpdateData(const void* _data, UInt32 _size) { buffer->UpdateData(_data, _size); };

		inline GPUBuffer* GetBuffer() const { return buffer.get(); }
		inline UInt32 GetSize() const { return buffer->GetSize(); }
		inline UInt32 GetStride() const { return stride; };

		static Shared<VertexBuffer> CreateDynamic(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize = 0, const void* _initialData = nullptr);
		static Shared<VertexBuffer> CreateStatic(UInt32 _size, UInt32 _stride, const void* _initialData);
	private:
		Shared<GPUBuffer> buffer;
		UInt32 stride;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(Shared<GPUBuffer> _buffer, UInt32 _indexCount);
		~IndexBuffer() {}

		inline GPUBuffer* GetBuffer() const { return buffer.get(); }

		UInt32 GetIndexCount() const { return indexCount; }

		static Shared<IndexBuffer> Create(const UInt32* _indices, UInt32 _count);
	private:
		Shared<GPUBuffer> buffer;
		UInt32 indexCount;

	};

	class ConstantBuffer
	{
	public:
		ConstantBuffer(Shared<GPUBuffer> _buffer);
		~ConstantBuffer() {}

		inline void UpdateData(const void* _data, UInt32 _size) { buffer->UpdateData(_data, _size); }

		inline GPUBuffer* GetBuffer() const { return buffer.get(); }
		UInt32 GetSize() { return buffer->GetSize(); }

		static Shared<ConstantBuffer> Create(UInt32 _size);
	protected:
		Shared<GPUBuffer> buffer;
		void* data;
	};


}

