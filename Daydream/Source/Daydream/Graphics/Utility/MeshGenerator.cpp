#include "DaydreamPCH.h"
#include "MeshGenerator.h"

namespace Daydream
{
	MeshData MeshGenerator::CreateQuad(const Vector2& _scale)
	{
		return MeshData();
	}

	MeshData MeshGenerator::CreatePlane(Float32 _width, Float32 _height, UInt32 _widthSegments, UInt32 _heightSegments)
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

	MeshData MeshGenerator::CreateSphere(const Float32 _radius, const UInt32 _numSlices, const UInt32 _numStacks)
	{
		MeshData sphereMeshData;

		const Float32 sectorStep = twoPI / _numSlices;
		const Float32 stackStep = PI / _numStacks;

		for (UInt32 i = 0; i <= _numStacks; i++)
		{
			Vector3 stackStartPoint = RotateAxisZRadian(Vector3(0.0f, -_radius, 0.0f), i * stackStep);
			for (UInt32 j = 0; j <= _numSlices; j++)
			{
				Vertex vertex;
				vertex.position = RotateAxisYRadian(stackStartPoint, j * -sectorStep);
				vertex.normal = vertex.position;
				vertex.normal = glm::normalize(vertex.normal);
				vertex.texCoord = Vector2(Float32(i) / _numSlices, 1.0f - Float32(j) / _numStacks);

				sphereMeshData.vertices.push_back(vertex);
			}
		}

		for (UInt32 i = 0; i < _numStacks; i++)
		{
			const int offset = (_numSlices + 1) * i; // 1줄에 numSlices보다 1개 더있음 왜냐하면 원래 있던 점을 넣어야 되기 때문에
			for (UInt32 j = 0; j < _numSlices; j++) 
			{
				sphereMeshData.indices.push_back(offset + j);
				sphereMeshData.indices.push_back(offset + _numSlices + 1 + j);
				sphereMeshData.indices.push_back(offset + _numSlices + 1 + j + 1);

				sphereMeshData.indices.push_back(offset + j);
				sphereMeshData.indices.push_back(offset + _numSlices + 1 + j + 1);
				sphereMeshData.indices.push_back(offset + j + 1);
			}
		}
		return sphereMeshData;
	}

}
