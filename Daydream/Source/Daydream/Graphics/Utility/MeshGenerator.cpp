#include "DaydreamPCH.h"
#include "MeshGenerator.h"

namespace Daydream
{
	MeshData MeshGenerator::CreateQuad(const Vector2& _scale)
	{
		return MeshData();
	}

	MeshData MeshGenerator::CreateCube(const Vector3& _dimension)
	{

		Float32 halfWidth = _dimension.x / 2.0f;
		Float32 halfHeight = _dimension.y / 2.0f;
		Float32 halfDepth = _dimension.z / 2.0f;

		Array<Vector3> positions =
		{
			//Front
			Vector3(-halfWidth, -halfHeight, -halfDepth),
			Vector3(-halfWidth, halfHeight, -halfDepth),
			Vector3(halfWidth, halfHeight, -halfDepth),
			Vector3(halfWidth, -halfHeight, -halfDepth),
			//Back
			Vector3(-halfWidth, -halfHeight, halfDepth),
			Vector3(-halfWidth, halfHeight, halfDepth),
			Vector3(halfWidth, halfHeight, halfDepth),
			Vector3(halfWidth, -halfHeight, halfDepth)
		};

		Array<Vector3> normals =
		{
			Vector3(0.0f,0.0f,-1.0f),//Front
			Vector3(0.0f,0.0f,1.0f), //Back
			Vector3(0.0f,1.0f,0.0f), //Top
			Vector3(0.0f,-1.0f,0.0f),//Bottom
			Vector3(-1.0f,0.0f,0.0f),//Left
			Vector3(1.0f,0.0f,0.0f), //Right
		};

		Array<Array<UInt32>> cubeFaces =
		{
			{0,1,2,3}, //Front
			{7,6,5,4}, //Back
			{1,5,6,2}, //Top
			{4,0,3,7}, //Bottom
			{4,5,1,0}, //Left
			{3,2,6,7}, //Right
		};

		Array<Vector2> texCoords =
		{
			Vector2(0.0f, 1.0f),
			Vector2(0.0f, 0.0f),
			Vector2(1.0f, 0.0f),
			Vector2(1.0f, 1.0f),
		};
		
		MeshData cubeMeshData;

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Vertex vertex;
				vertex.position = positions[cubeFaces[i][j]];
				vertex.normal = normals[i];
				vertex.texCoord = texCoords[j];
				cubeMeshData.vertices.push_back(vertex);
			}
			
			UInt32 baseVertexIndex = i * 4;

			cubeMeshData.indices.push_back(baseVertexIndex + 0);
			cubeMeshData.indices.push_back(baseVertexIndex + 1);
			cubeMeshData.indices.push_back(baseVertexIndex + 2);

			cubeMeshData.indices.push_back(baseVertexIndex + 0);
			cubeMeshData.indices.push_back(baseVertexIndex + 2);
			cubeMeshData.indices.push_back(baseVertexIndex + 3);
		}
		return cubeMeshData;
	}
	MeshData MeshGenerator::CreateCube(Float32 _width, Float32 _height, Float32 _depth)
	{
		return CreateCube(Vector3(_width, _height, _depth));
	}
	MeshData MeshGenerator::CreateCube(Float32 _scale)
	{
		return CreateCube(_scale, _scale, _scale);
	}

}
