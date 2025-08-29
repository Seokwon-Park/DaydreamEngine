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

	struct MaterialData
	{
		std::string name;

		// �ؽ�ó ���� ��ε�
		std::string diffuseTexturePath;
		std::string specularTexturePath;
		std::string normalMapPath;
		// ... PBR �ؽ�ó ��ε� (metallic, roughness, ao ...)

		// �ؽ�ó�� �ƴ� ��� ����
		Vector3 diffuseColor = { 0.8f, 0.8f, 0.8f };
		float shininess = 32.0f;
		// ... PBR ��� ���� (metallicFactor, roughnessFactor ...)
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
		UInt32 GetIndexCount() { return indexBuffer->GetCount(); }

		Shared<Texture2D> GetDiffuseTexture() { return diffuseTexture; }

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