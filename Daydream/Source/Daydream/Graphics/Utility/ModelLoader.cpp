#include "DaydreamPCH.h"
#include "ModelLoader.h"

namespace Daydream
{

	Array<MeshData> ModelLoader::LoadFromFileInternal(const Path& _filepath)
	{
		Assimp::Importer importer;

		Path basePath = _filepath;
		basePath.MoveToParent();
		baseDirectory = basePath;

		UInt32 flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;// |           // 모든 면을 삼각형으로 변환
		//aiProcess_FlipUVs |              // UV 좌표 뒤집기 (OpenGL용)
		//aiProcess_GenNormals |           // 노말 벡터 생성
		//aiProcess_CalcTangentSpace |     // 탄젠트/바이탄젠트 계산
		//aiProcess_JoinIdenticalVertices | // 중복 정점 제거
		//aiProcess_OptimizeMeshes |       // 메시 최적화
		//aiProcess_ValidateDataStructure;  // 데이터 유효성 검사

		DAYDREAM_INFO(_filepath.IsExistPath());

		const aiScene* scene = importer.ReadFile(_filepath.ToString(), flags);

		bool result = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;
		DAYDREAM_CORE_ASSERT(nullptr != scene, "{0}", importer.GetErrorString());
		return ProcessScene(scene);
	}
	Array<MeshData> ModelLoader::ProcessScene(const aiScene* _scene)
	{
		Array<MeshData> meshes;
		//Start form root node
		ProcessNode(_scene->mRootNode, _scene, meshes);
		return meshes;
	}
	void ModelLoader::ProcessNode(aiNode* _node, const aiScene* _scene, Array<MeshData>& _meshes)
	{
		for (UInt32 i = 0; i < _node->mNumMeshes; i++)
		{
			aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, _scene));
		}

		for (UInt32 i = 0; i < _node->mNumChildren; i++)
		{
			ProcessNode(_node->mChildren[i], _scene, _meshes);
		}
	}
	MeshData ModelLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
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

		if (_mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];

			ProcessMaterial(material, meshData.materialData);
		}
		return meshData;
	}
	void ModelLoader::ProcessMaterial(aiMaterial* _material, MaterialData& _outMaterialData)
	{
		// 1. 재질 이름 가져오기
		aiString name;
		_material->Get(AI_MATKEY_NAME, name);
		_outMaterialData.name = name.C_Str();

		// 2. 텍스처 경로들 가져오기
		LoadMaterialTexture(_material, aiTextureType_DIFFUSE, _outMaterialData.diffuseTexturePath);
		LoadMaterialTexture(_material, aiTextureType_SPECULAR, _outMaterialData.specularTexturePath);
		LoadMaterialTexture(_material, aiTextureType_NORMALS, _outMaterialData.normalMapPath);
		// ... 다른 PBR 텍스처들도 같은 방식으로 로드 ...

		// 3. 상수 값들 가져오기
		aiColor4D diffuseColor;
		if (aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
		{
			_outMaterialData.diffuseColor = Vector3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
		}

		float shininess;
		if (aiGetMaterialFloat(_material, AI_MATKEY_SHININESS, &shininess) == AI_SUCCESS)
		{
			_outMaterialData.shininess = shininess;
		}
	}

	void ModelLoader::LoadMaterialTexture(aiMaterial* _mat, aiTextureType _type, std::string& _outPath)
	{
		// 해당 타입의 텍스처가 1개 이상 있는지 확인하고, 첫 번째 텍스처 경로를 가져옴
		if (_mat->GetTextureCount(_type) > 0)
		{
			aiString str;
			if (_mat->GetTexture(_type, 0, &str) == AI_SUCCESS)
			{
				_outPath = (baseDirectory + str.C_Str()).ToString();
			}
		}
	}
}