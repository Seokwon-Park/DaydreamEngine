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
		static ModelLoader* GetInstance()
		{
			return instance;
		}

		static void Init() { instance = new ModelLoader(); }
		static void Shutdown() { delete instance; };

		static ModelData LoadFromFile(const Path& _filepath) { return instance->LoadFromFileInternal(_filepath); }
	private:
		inline static ModelLoader* instance;

		ModelLoader() = default;
		ModelData LoadFromFileInternal(const Path& _filepath);
		void ProcessScene(const aiScene* _scene);
		void ProcessNode(aiNode* _node, const aiScene* _scene);
		void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
		void GetTexturePath(aiMaterial* _mat, aiTextureType _type, std::string& _outPath);
		void ProcessMaterial(aiMaterial* _material);

		Path baseDirectory;
		ModelData modelData;
	};
}