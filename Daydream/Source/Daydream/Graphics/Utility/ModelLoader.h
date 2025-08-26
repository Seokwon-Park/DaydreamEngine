#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Daydream/Graphics/Resources/Mesh.h"

namespace Daydream
{
	class ModelLoader
	{
	public:
		static Array<MeshData> LoadFromFile(const FilePath& _filepath);
	private:
		static Array<MeshData> ProcessScene(const aiScene* _scene);
		static void ProcessNode(aiNode* _node, const aiScene* _scene, Array<MeshData>& _meshes);
		static MeshData ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

	};
}