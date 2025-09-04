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
		static ModelLoader GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		static Array<MeshData> LoadFromFile(const Path& _filepath) { return GetInstance().LoadFromFileInternal(_filepath); }
	private:
		ModelLoader() = default;
		Array<MeshData> LoadFromFileInternal(const Path& _filepath);
		void ProcessScene(const aiScene* _scene);
		void ProcessNode(aiNode* _node, const aiScene* _scene);
		void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
		void GetTexturePath(aiMaterial* _mat, aiTextureType _type, std::string& _outPath);
		void ProcessMaterial(aiMaterial* _material);
		
		Path baseDirectory;
		Array<MeshData> modelData;
	};
}