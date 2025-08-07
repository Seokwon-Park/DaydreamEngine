#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Material.h"

namespace Daydream
{
	class Mesh
	{
	public:
		Mesh() {};
		Mesh(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer);

		~Mesh();

		void SetVertexBuffer(Shared<VertexBuffer> _vertexBuffer);
		void GetVertexBuffer(Shared<VertexBuffer> _vertexBuffer) {};
		void SetIndexBuffer(Shared<IndexBuffer> _indexBuffer);
		UInt32 GetIndexCount() { return indexBuffer->GetCount(); }

		void Bind();

		static Shared<Mesh> Create();
		static Shared<Mesh> Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer);

	private:
		Shared<VertexBuffer> vertexBuffer;
		Shared<IndexBuffer> indexBuffer;
	};
}