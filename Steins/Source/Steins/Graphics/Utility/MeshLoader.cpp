#include "SteinsPCH.h"
#include "MeshLoader.h"



namespace Steins
{
	std::vector<MeshData> MeshLoader::LoadFromFile(const FilePath& _filepath)
	{
		Assimp::Importer importer;

		UInt32 flags = aiProcess_Triangulate |           // 모든 면을 삼각형으로 변환
			//aiProcess_FlipUVs |              // UV 좌표 뒤집기 (OpenGL용)
			aiProcess_GenNormals |           // 노말 벡터 생성
			aiProcess_CalcTangentSpace |     // 탄젠트/바이탄젠트 계산
			aiProcess_JoinIdenticalVertices | // 중복 정점 제거
			aiProcess_OptimizeMeshes |       // 메시 최적화
			aiProcess_ValidateDataStructure;  // 데이터 유효성 검사

		const aiScene* scene = importer.ReadFile(_filepath.GetCurrentPath().c_str(), flags);

		bool result = !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode;
		STEINS_CORE_ASSERT(nullptr != scene, "{0}", importer.GetErrorString());
		return ProcessScene(scene);
	}
	std::vector<MeshData> MeshLoader::ProcessScene(const aiScene* _scene)
	{
		std::vector<MeshData> meshes;
		//Start form root node
		ProcessNode(_scene->mRootNode, _scene, meshes);
		return meshes;
	}
	void MeshLoader::ProcessNode(aiNode* _node, const aiScene* _scene, std::vector<MeshData>& _meshes)
	{
		for (UInt32 i = 0; i < _node->mNumMeshes; i++)
		{
			//node->mMeshes[i]는 scene에서의 node의 Mesh인덱스
			aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
			_meshes.push_back(ProcessMesh(mesh, _scene));
		}
	}
	MeshData MeshLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
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
		return MeshData();
	}
}