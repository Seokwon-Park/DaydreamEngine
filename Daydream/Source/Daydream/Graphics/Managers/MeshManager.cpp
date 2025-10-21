#include "DaydreamPCH.h"
#include "MeshManager.h"

#include "Daydream/Graphics/Utility/MeshGenerator.h"

namespace Daydream
{
	MeshManager::MeshManager()
	{
		
	}
	void MeshManager::CreateEssentialMeshes()
	{
		float vertices[] = 
		{
			-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		};

		UInt32 indices[6] = { 0,1,2,2,3,0 };
		auto vertexBuffer = VertexBuffer::CreateStatic(sizeof(vertices), 20, vertices);
		auto indexBuffer = IndexBuffer::Create(indices, 6);
		
		registry["Quad"] = Mesh::Create(vertexBuffer, indexBuffer);

		auto meshData = MeshGenerator::CreateCube();
		Array<Vector3> positions;
		for (Vertex v : meshData.vertices)
		{
			positions.push_back(v.position);
		}
		vertexBuffer = VertexBuffer::CreateStatic(sizeof(Vector3) * positions.size(), 12, positions.data());
		indexBuffer = IndexBuffer::Create(meshData.indices.data(), meshData.indices.size());
		registry["Box"] = Mesh::Create(vertexBuffer, indexBuffer);
	}
}
