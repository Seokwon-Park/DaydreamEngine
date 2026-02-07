#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"

namespace Daydream
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(UInt32 _size, UInt32 _stride, const void* _vertices);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void* GetNativeHandle() const override { return reinterpret_cast<void*>(static_cast<UInt64>(bufferID)); };
		virtual void SetData(const void * _data, UInt32 _dataSize) override;

		UInt32 GetBufferID() const { return bufferID; }
	private:
		UInt32 bufferID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const UInt32* _indices, UInt32 _IndexCount);
		virtual ~OpenGLIndexBuffer();

		virtual void* GetNativeHandle() const override { return reinterpret_cast<void*>(static_cast<UInt64>(bufferID)); };
		virtual void Bind() const;
		virtual void Unbind() const;

		UInt32 GetBufferID() const { return bufferID; }
	private:
		UInt32 bufferID;
	};

	class OpenGLConstantBuffer: public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(UInt32 _size);
		virtual ~OpenGLConstantBuffer();

		//virtual void Bind(UInt32 _slot) const override;

		virtual void Update(const void* _data, UInt32 _size) override;

		virtual void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<uintptr_t>(bufferID)); }

		UInt32 GetBufferID() const { return bufferID; }
	private:
		UInt32 bufferID;
	};
}
