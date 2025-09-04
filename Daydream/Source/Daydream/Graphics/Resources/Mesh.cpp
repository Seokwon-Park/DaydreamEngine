#include "DaydreamPCH.h"
#include "Mesh.h"
#include "Daydream/Graphics/Utility/ModelLoader.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Core/ResourceManager.h"

namespace Daydream
{
	Mesh::Mesh(const MeshData& _meshData)
	{
		vertexBuffer = VertexBuffer::CreateStatic(_meshData.vertices.size() * sizeof(Vertex), sizeof(Vertex), _meshData.vertices.data());
		indexBuffer = IndexBuffer::Create(_meshData.indices.data(), _meshData.indices.size());

		if (!_meshData.materialData.diffuseTexturePath.empty())
			diffuseTexture = ResourceManager::GetResource<Texture2D>(_meshData.materialData.diffuseTexturePath);
		if (!_meshData.materialData.normalMapPath.empty())
			normalTexture = ResourceManager::GetResource<Texture2D>(_meshData.materialData.normalMapPath);
	}
	Mesh::Mesh(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer)
	{
		vertexBuffer = _vertexBuffer;
		indexBuffer = _indexBuffer;
	}

	Mesh::~Mesh()
	{
		vertexBuffer.reset();
		indexBuffer.reset();
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

	Shared<Mesh> Mesh::Create(const MeshData& _meshData)
	{
		return MakeShared<Mesh>(_meshData);
	}

	Shared<Mesh> Mesh::Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer)
	{
		return MakeShared<Mesh>(_vertexBuffer, _indexBuffer);
	}
}