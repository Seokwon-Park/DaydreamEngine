#include "DaydreamPCH.h"
#include "ModelLoader.h"

namespace Daydream
{
	namespace
	{
		Matrix4x4 ConvertAssimpMatrix(const aiMatrix4x4& _mat)
		{
			// 엔진의 행렬 메모리 레이아웃(Row/Column Major)에 맞춰 변환
			Matrix4x4 ret;

			ret.glmMat = glm::make_mat4(&_mat.a1);
			ret.glmMat = glm::transpose(ret.glmMat);

			return ret;
		}

		bool CheckIsSkeletal(const aiScene* _scene)
		{
			// 1. 애니메이션이 있으면 무조건 스켈레탈로 간주
			if (_scene->mNumAnimations > 0) return true;

			// 2. 메쉬들을 뒤져서 뼈가 있는지 확인
			for (unsigned int i = 0; i < _scene->mNumMeshes; i++)
			{
				aiMesh* mesh = _scene->mMeshes[i];
				if (mesh->HasBones())
				{
					return true;
				}
			}

			return false;
		}
	}

	Shared<ModelData> ModelLoader::LoadFromFile(const Path& _filepath)
	{
		Assimp::Importer importer;

		Path baseDirectory;
		baseDirectory = _filepath.parent_path();

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

		Shared<ModelData> modelData;

		modelData = MakeShared<ModelData>();
		modelData->meshes.clear();
		modelData->materials.clear();

		if (scene->mNumMaterials > 0)
			modelData->materials.reserve(scene->mNumMaterials);
		if (scene->mNumMeshes > 0)
			modelData->meshes.reserve(scene->mNumMeshes);

		for (UInt32 i = 0; i < scene->mNumMaterials; i++)
		{
			ProcessMaterial(scene->mMaterials[i], modelData, baseDirectory);
		}

		for (UInt32 i = 0; i < scene->mNumMeshes; i++)
		{
			ProcessMesh(scene->mMeshes[i], scene, modelData);
		}

		modelData->rootNode = ProcessNode(scene->mRootNode, scene, modelData);
		return modelData;
	}
	
	void ModelLoader::ProcessScene(const aiScene* _scene)
	{
		ProcessNode(_scene->mRootNode, _scene, nullptr);
	}
	NodeData ModelLoader::ProcessNode(aiNode* _node, const aiScene* _scene, Shared<ModelData> _modelData)
	{
		NodeData node;
		node.name = _node->mName.C_Str();

		// 핵심: Assimp의 변환 행렬을 엔진의 Matrix4로 변환하여 저장
		// (Sponza의 기둥 위치 등이 여기에 포함됨)
		node.transform = ConvertAssimpMatrix(_node->mTransformation);

		// 해당 노드가 참조하는 메쉬 인덱스 저장
		for (UInt32 i = 0; i < _node->mNumMeshes; i++)
		{
			// _node->mMeshes[i]는 scene->mMeshes 배열의 인덱스임
			node.meshIndices.push_back(_node->mMeshes[i]);
		}

		// 자식 노드 순회
		for (UInt32 i = 0; i < _node->mNumChildren; i++)
		{
			NodeData childNode = ProcessNode(_node->mChildren[i], _scene, _modelData);
			node.children.push_back(childNode);
		}

		return node;
	}
	void ModelLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene, Shared<ModelData> _modelData)
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
		meshData.materialIndex = _mesh->mMaterialIndex;

		_modelData->meshes.push_back(meshData);
	}
	void ModelLoader::ProcessMaterial(aiMaterial* _material, Shared<ModelData> _modelData, const Path& _baseDirectory)
	{
		MaterialData materialData;
		aiString name;
		_material->Get(AI_MATKEY_NAME, name);
		materialData.name = name.C_Str();

		materialData.albedoMapPath = StringGetTexturePath(_material, aiTextureType_DIFFUSE, _baseDirectory);
		//GetTexturePath(_material, aiTextureType_SPECULAR, materialData.specularTexturePath);
		materialData.normalMapPath = StringGetTexturePath(_material, aiTextureType_NORMALS, _baseDirectory);
		materialData.roughnessMapPath = StringGetTexturePath(_material, aiTextureType_DIFFUSE_ROUGHNESS, _baseDirectory);
		materialData.metallicMapPath = StringGetTexturePath(_material, aiTextureType_METALNESS, _baseDirectory);
		materialData.AOMapPath = StringGetTexturePath(_material, aiTextureType_AMBIENT_OCCLUSION, _baseDirectory);

		if (materialData.AOMapPath.empty())
		{
			if (!materialData.roughnessMapPath.empty() &&
				materialData.roughnessMapPath == materialData.metallicMapPath)
			{
				// "이 텍스처는 ORM 압축 텍스처다!"라고 추론합니다.
				// AO 맵 경로를 Roughness/Metallic 맵 경로와 동일하게 "강제"로 설정합니다.
				// DAYDREAM_CORE_INFO("AO 맵 경로를 Packed Texture({0})로 추론합니다.", materialData.roughnessMapPath);
				materialData.AOMapPath = materialData.roughnessMapPath;
			}
			else
			{
				materialData.AOMapPath = StringGetTexturePath(_material, aiTextureType_LIGHTMAP, _baseDirectory);
			}
		}
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
		_modelData->materials.push_back(materialData);
	}

	String ModelLoader::StringGetTexturePath(aiMaterial* _mat, aiTextureType _type, const Path& _baseDirectory)
	{
		// 해당 타입의 텍스처가 1개 이상 있는지 확인하고, 첫 번째 텍스처 경로를 가져옴
		if (_mat->GetTextureCount(_type) > 0)
		{
			aiString str;
			if (_mat->GetTexture(_type, 0, &str) == AI_SUCCESS)
			{
				return  (_baseDirectory / str.C_Str()).generic_string();
			}
		}
		return "";
	}
}