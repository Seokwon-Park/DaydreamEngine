#pragma once

#include "Steins/Graphics/Resources/Buffer.h"

namespace Steins
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(UInt32 _bufferSize, UInt32 _stride);
		OpenGLVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void SetData(void* _data, UInt32 _dataSize) override {}
	private:
		UInt32 rendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(UInt32* _indices, UInt32 _IndexCount);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		UInt32 rendererID;
	};

	class OpenGLConstantBuffer: public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(UInt32 _size);
		virtual ~OpenGLConstantBuffer();

		virtual void Bind(UInt32 _slot, ShaderStage _flags) const override;

		virtual void Update(const void* _data, UInt32 _size) override;

		virtual void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<uintptr_t>(bufferID)); }

	private:
		UInt32 bufferID;
	};
}
