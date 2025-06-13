#pragma once

namespace Steins
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoord;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<UInt32> indices;
		std::string name;
	};

	class MeshLoader
	{
	public:
		static std::vector<MeshData> LoadFromFile(const FilePath& _filepath);
	private:
		static std::vector<MeshData> ProcessScene(const aiScene* _scene);
		static void ProcessNode(aiNode* _node, const aiScene* _scene, std::vector<MeshData>& _meshes);
		static MeshData ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

	};
}