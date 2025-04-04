#pragma once

#include "Buffer.h"

namespace Steins {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer) = 0;

		virtual const Array<Shared<VertexBuffer>>& GetVertexBuffers() const {return vertexBuffers;}
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; };

		static Shared<VertexArray> Create();
	protected:
		Array<Shared<VertexBuffer>> vertexBuffers;
		Shared<IndexBuffer> indexBuffer;
	};

}