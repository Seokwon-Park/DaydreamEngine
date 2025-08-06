#include "DaydreamPCH.h"
#include "Mesh.h"
#include "Daydream/Graphics/Utility/ModelLoader.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Mesh::Mesh(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer)
	{
		vertexBuffer = _vertexBuffer;
		indexBuffer = _indexBuffer;
	}

	void Mesh::SetVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		vertexBuffer = _vertexBuffer;
	}
	void Mesh::SetIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		indexBuffer = _indexBuffer;
	}

	void Mesh::Bind()
	{
		vertexBuffer->Bind();
		indexBuffer->Bind();
	}
	Shared<Mesh> Mesh::Create()
	{
		return MakeShared<Mesh>();
	}

	Shared<Mesh> Mesh::Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer)
	{
		return MakeShared<Mesh>(_vertexBuffer, _indexBuffer);
	}
}