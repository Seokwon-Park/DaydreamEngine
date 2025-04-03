#pragma once


namespace Steins
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(Float32* _vertices, UInt32 _size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(UInt32* _vertices, UInt32 _size);
	};
}

