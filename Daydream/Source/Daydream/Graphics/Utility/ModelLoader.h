#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class ModelLoader
	{
	public:
		static Shared<ModelData> LoadFromFile(const Path& _filepath);
	private:
		ModelLoader() = default;
		static void ProcessScene(const aiScene* _scene);
		static NodeData ProcessNode(aiNode* _node, const aiScene* _scene, Shared<ModelData> _modelData);
		static void ProcessMesh(aiMesh* _mesh, const aiScene* _scene, Shared<ModelData> _modelData);
		static void ProcessMaterial(aiMaterial* _material, Shared<ModelData> _modelData, const Path& _baseDirectory);
		static String StringGetTexturePath(aiMaterial* _mat, aiTextureType _type, const Path& _baseDirectory);
	};
}