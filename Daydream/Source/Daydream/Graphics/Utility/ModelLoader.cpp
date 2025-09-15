#include "DaydreamPCH.h"
#include "ModelLoader.h"

namespace Daydream
{

	ModelData ModelLoader::LoadFromFileInternal(const Path& _filepath)
	{
		Assimp::Importer importer;

		Path basePath = _filepath;
		basePath = basePath.parent_path();
		baseDirectory = basePath;

		modelData.meshes.clear();
		modelData.materials.clear();

		UInt32 flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;// |           // 모든 면을 삼각형으로 변환
			//aiProcess_FlipUVs |				// UV 좌표 뒤집기 (OpenGL용)
			//aiProcess_GenNormals |			// 노말 벡터 생성
		//	aiProcess_CalcTangentSpace;			// | //탄젠트/바이탄젠트 계산
		//aiProcess_JoinIdenticalVertices | // 중복 정점 제거
		//aiProcess_OptimizeMeshes |		// 메시 최적화
		//aiProcess_ValidateDataStructure;  // 데이터 유효성 검사

		DAYDREAM_INFO(FileSystem::exists(_filepath));

		const aiScene* scene = importer.ReadFile(_filepath.string(), flags);

		bool result = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;
		DAYDREAM_CORE_ASSERT(nullptr != scene, "{0}", importer.GetErrorString());

		ProcessScene(scene);
		return modelData;
	}
	void ModelLoader::ProcessScene(const aiScene* _scene)
	{
		ProcessNode(_scene->mRootNode, _scene);
	}
	void ModelLoader::ProcessNode(aiNode* _node, const aiScene* _scene)
	{
		for (UInt32 i = 0; i < _node->mNumMeshes; i++)
		{
			aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
			ProcessMesh(mesh, _scene);
		}

		for (UInt32 i = 0; i < _node->mNumChildren; i++)
		{
			ProcessNode(_node->mChildren[i], _scene);
		}
	}
	void ModelLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
	{
		MeshData meshData;
		meshData.name = _mesh->mName.C_Str();

		for (UInt32 i = 0; i < _mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position.x = _mesh->mVertices[i].x;
			vertex.position.y = _mesh->mVertices[i].y;
			vertex.position.z = _mesh->mVertices[i].z;

			if (_mesh->HasNormals())
			{
				vertex.normal.x = _mesh->mNormals[i].x;
				vertex.normal.y = _mesh->mNormals[i].y;
				vertex.normal.z = _mesh->mNormals[i].z;
			}

			if (_mesh->HasTangentsAndBitangents())
			{
				vertex.tangent.x = _mesh->mTangents[i].x;
				vertex.tangent.y = _mesh->mTangents[i].y;
				vertex.tangent.z = _mesh->mTangents[i].z;
			}

			if (_mesh->mTextureCoords[0])
			{
				vertex.texCoord.x = _mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = _mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertex.texCoord = Vector2(0.0f, 0.0f);
			}

			meshData.vertices.push_back(vertex);
		}


		for (UInt32 i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace face = _mesh->mFaces[i];
			for (UInt32 j = 0; j < face.mNumIndices; j++)
			{
				meshData.indices.push_back(face.mIndices[j]);
			}
		}

		modelData.meshes.push_back(meshData);

		// TODO: Fix 만약 materialIndex가 0이면 mesh랑 서로 안맞게됨
		if (_mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];

			ProcessMaterial(material);
		}
	}
	void ModelLoader::ProcessMaterial(aiMaterial* _material)
	{
		MaterialData materialData;
		aiString name;
		_material->Get(AI_MATKEY_NAME, name);
		materialData.name = name.C_Str();

		GetTexturePath(_material, aiTextureType_DIFFUSE, materialData.diffuseTexturePath);
		GetTexturePath(_material, aiTextureType_SPECULAR, materialData.specularTexturePath);
		GetTexturePath(_material, aiTextureType_NORMALS, materialData.normalMapPath);

		aiColor4D diffuseColor;
		if (aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
		{
			materialData.diffuseColor = Vector3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
		}

		float shininess;
		if (aiGetMaterialFloat(_material, AI_MATKEY_SHININESS, &shininess) == AI_SUCCESS)
		{
			materialData.shininess = shininess;
		}
		modelData.materials.push_back(materialData);
	}

	void ModelLoader::GetTexturePath(aiMaterial* _mat, aiTextureType _type, std::string& _outPath)
	{
		// 해당 타입의 텍스처가 1개 이상 있는지 확인하고, 첫 번째 텍스처 경로를 가져옴
		if (_mat->GetTextureCount(_type) > 0)
		{
			aiString str;
			if (_mat->GetTexture(_type, 0, &str) == AI_SUCCESS)
			{
				_outPath = (baseDirectory / str.C_Str()).make_preferred().string();
			}
		}
	}
}