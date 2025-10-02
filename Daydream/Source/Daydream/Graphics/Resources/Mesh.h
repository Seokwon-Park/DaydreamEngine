#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Material.h"

namespace Daydream
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector3 tangent;
		Vector2 texCoord;
	};

	struct MeshData
	{
		Array<Vertex> vertices;
		Array<UInt32> indices;
		MaterialData materialData;
		Path basePath;
		std::string name;
	};

	class Mesh
	{
	public:
		Mesh() {};
		Mesh(const MeshData& _meshData);
		Mesh(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer);
		~Mesh();

		Shared<VertexBuffer> GetVertexBuffer() { return vertexBuffer; }
		UInt32 GetIndexCount() { return indexBuffer->GetIndexCount(); }

		Shared<Texture2D> GetDiffuseTexture() { return diffuseTexture; }
		Shared<Texture2D> GetNormalTexture() { return normalTexture; }

		void Bind();

		static Shared<Mesh> Create();
		static Shared<Mesh> Create(const MeshData& _meshData);
		static Shared<Mesh> Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer);
	private:
		Shared<VertexBuffer> vertexBuffer;
		Shared<IndexBuffer> indexBuffer;

		Shared<Texture2D> diffuseTexture;
		Shared<Texture2D> specularTexture;
		Shared<Texture2D> normalTexture;
	};
}