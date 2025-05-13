#pragma once

#include "Steins/Render/Buffer.h"

namespace Steins
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(Float32* _vertices, UInt32 _size, const BufferLayout& _layout);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t rendererID;
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
