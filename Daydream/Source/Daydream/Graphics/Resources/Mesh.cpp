#include "DaydreamPCH.h"
#include "Mesh.h"
#include "Daydream/Graphics/Utility/ModelLoader.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Mesh::Mesh(const MeshData& _meshData)
	{
		vertexBuffer = VertexBuffer::CreateStatic(_meshData.vertices.size() * sizeof(Vertex), sizeof(Vertex), _meshData.vertices.data());
		indexBuffer = IndexBuffer::Create(_meshData.indices.data(), _meshData.indices.size());

		TextureDesc textureDesc{};
		textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
		textureDesc.format = Daydream::RenderFormat::R8G8B8A8_UNORM_SRGB;
		if (!_meshData.materialData.diffuseTexturePath.empty())
			diffuseTexture = Texture2D::Create(_meshData.materialData.diffuseTexturePath, textureDesc);
		else
		{
			DAYDREAM_CORE_ASSERT(false, "Breakpoint");
		}
		//if (!_meshData.materialData.normalMapPath.empty())
		//	normalTexture = Texture2D::Create(_meshData.materialData.normalMapPath, textureDesc);
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