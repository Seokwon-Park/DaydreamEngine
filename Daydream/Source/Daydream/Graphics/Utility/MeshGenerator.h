#pragma once

#include "Daydream/Graphics/Resources/Mesh.h"

namespace Daydream
{
	class MeshGenerator
	{
	public:
		static MeshData CreateQuad(const Vector2& _scale);

		//width, height, depth(x,y,z)
		static MeshData CreateCube(const Vector3& _dimension);
		static MeshData CreateCube(Float32 _width, Float32 _height, Float32 _depth);
		//_scale is not Transform scale
		static MeshData CreateCube(Float32 _scale = 1.0f);
	};
}