#pragma once

#include "Daydream/Graphics/Resources/Mesh.h"

namespace Daydream
{
	class MeshGenerator
	{
	public:
		static MeshData CreateQuad(const Vector2& _scale);
		static MeshData CreatePlane(Float32 _width, Float32 _height, UInt32 _widthSegments, UInt32 _heightSegments);

		//width, height, depth(x,y,z)
		static MeshData CreateCube(const Vector3& _dimension);
		static MeshData CreateCube(Float32 _width, Float32 _height, Float32 _depth);
		//_scale is not Transform scale
		static MeshData CreateCube(Float32 _scale = 1.0f);

		static MeshData CreateCone(Float32 _radius, Float32 _height, Float32 _numslices) {};
		static MeshData CreateCylinder(Float32 _radius, Float32 _height, Float32 _numslices) {};

		static MeshData CreateSphere(Float32 _radius, UInt32 _numSlices,
			UInt32 _numStacks);
	};
}