#pragma once

#include "Steins/Render/VertexArray.h"

namespace Steins
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer) override;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer) override;

		virtual const Array<Shared<VertexBuffer>>& GetVertexBuffers() const override { return vertexBuffers; }
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer; };
	private:
		UInt32 rendererID;
	};
}

