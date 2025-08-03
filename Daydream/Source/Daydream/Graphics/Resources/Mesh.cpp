#include "DaydreamPCH.h"
#include "Mesh.h"
#include "Daydream/Graphics/Utility/ModelLoader.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Mesh::Mesh(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer, Shared<Material> _material)
	{
		vertexBuffer = _vertexBuffer;
		indexBuffer = _indexBuffer;
		material = _material;
	}

	void Mesh::SetVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		vertexBuffer = _vertexBuffer;
	}
	void Mesh::SetIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		indexBuffer = _indexBuffer;
	}

	void Mesh::Draw()
	{
		vertexBuffer->Bind();
		indexBuffer->Bind();
		material->Bind();

		Renderer::Submit(indexBuffer->GetCount());
	}
	Shared<Mesh> Mesh::Create()
	{
		return MakeShared<Mesh>();
	}

	Shared<Mesh> Mesh::Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer, Shared<Material> _material)
	{
		return MakeShared<Mesh>(_vertexBuffer, _indexBuffer, _material);
	}
}