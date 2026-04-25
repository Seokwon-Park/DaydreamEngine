#pragma once

#include "Daydream/Enum/RendererEnums.h"

namespace Daydream
{
	struct BufferDesc
	{
		UInt32 size = 0;
		BufferUsage bufferUsage = BufferUsage::None;
		MemoryUsage memoryUsage = MemoryUsage::Static;
	};

	class GPUBuffer
	{
	public:
		GPUBuffer(const BufferDesc& _desc);
		virtual ~GPUBuffer() {}

		virtual void* Map() = 0;
		virtual void Unmap() = 0;
		virtual void UpdateData(const void* _data, UInt32 _size) = 0;

		const BufferDesc& GetDesc() const { return desc; }
		
		/*static Shared<GPUBuffer> Create(const BufferDesc& _desc);*/
	protected:
		BufferDesc desc;
		void* mappedData;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() const = 0;
		virtual void SetData(const void* _data, UInt32 _dataSize) = 0;

		inline UInt32 GetStride() const { return stride; };

		static Shared<VertexBuffer> CreateDynamic(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize = 0, const void* _initialData = nullptr);
		static Shared<VertexBuffer> CreateStatic(UInt32 _size, UInt32 _stride, const void* _initialData);
	protected:
		UInt32 stride;
		MemoryUsage usage;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() const = 0;
		UInt32 GetIndexCount() const { return indexCount; }

		static Shared<IndexBuffer> Create(const UInt32* _indices, UInt32 _count);
	protected:
		UInt32 indexCount;

	};

	class ConstantBuffer
	{
	public:
		virtual ~ConstantBuffer() {}

		//virtual void Bind(UInt32 _slot) const = 0;

		virtual void Update(const void* _data, UInt32 _size) = 0;

		virtual void* GetNativeHandle() = 0;
		UInt32 GetSize() { return size; }

		static Shared<ConstantBuffer> Create(UInt32 _size);
	protected:
		void* data;
		UInt32 size;
	};


}

