#include "DaydreamPCH.h"
#include "Model.h"

#include "Daydream/Graphics/Utility/ModelLoader.h"
#include "Daydream/Graphics/Resources/Buffer.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Model::Model()
	{
	}
	void Model::Load(const String& _path)
	{
		FilePath filepath(_path);

		Array<MeshData> meshes = ModelLoader::LoadFromFile(filepath);
		for (auto meshData : meshes)
		{
			Shared<VertexBuffer> vertexBuffer = VertexBuffer::CreateStatic(meshData.vertices.data(), meshData.vertices.size() * sizeof(Vertex), sizeof(Vertex));
			Shared<IndexBuffer> indexBuffer = IndexBuffer::Create(meshData.indices.data(), meshData.indices.size());

			Renderer::Submit(indexBuffer->GetCount());
		}
	}
}
