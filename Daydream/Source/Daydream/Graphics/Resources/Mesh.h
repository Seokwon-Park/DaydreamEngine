#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Material.h"

namespace Daydream
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoord;
	};

	struct MeshData
	{
		Array<Vertex> vertices;
		Array<UInt32> indices;
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
		UInt32 GetIndexCount() { return indexBuffer->GetCount(); }

		void Bind();

		static Shared<Mesh> Create();
		static Shared<Mesh> Create(const MeshData& _meshData);
		static Shared<Mesh> Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer);
	private:


		Shared<VertexBuffer> vertexBuffer;
		Shared<IndexBuffer> indexBuffer;
	};
}