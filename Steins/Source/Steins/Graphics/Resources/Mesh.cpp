#include "SteinsPCH.h"
#include "Mesh.h"
#include <Steins/Graphics/Utility/MeshLoader.h>

#include "Steins/Graphics/Core/Renderer.h"

namespace Steins
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
	void Mesh::Load(MeshData _meshData)
	{
		//FilePath filepath(_filepath);
		//auto meshData = MeshLoader::LoadFromFile(filepath);

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