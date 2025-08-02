#pragma once

#include "Steins/Graphics/Resources/Buffer.h"
#include "Steins/Graphics/Resources/Material.h"

namespace Steins
{
	class Mesh
	{
	public:
		Mesh() {};
		Mesh(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer, Shared<Material> _material);

		void SetVertexBuffer(Shared<VertexBuffer> _vertexBuffer);
		void SetIndexBuffer(Shared<IndexBuffer> _indexBuffer);
		void SetMaterial(Shared<Material> _material) { material = _material; }
		Shared<Material> GetMaterial() { return material; }
		void Load(MeshData _meshData);

		void Draw();

		static Shared<Mesh> Create();
		static Shared<Mesh> Create(Shared<VertexBuffer> _vertexBuffer, Shared<IndexBuffer> _indexBuffer, Shared<Material> _material);
	private:
		Shared<VertexBuffer> vertexBuffer;
		Shared<IndexBuffer> indexBuffer;
		Shared<Material> material;
	};
}