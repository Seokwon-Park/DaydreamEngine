#pragma once


#include "TypeTraits.h"

#include "glm/glm.hpp"


namespace Steins
{
	class Vector3
	{
	public:
		union
		{
			struct
			{
				Float32 x;
				Float32 y;
				Float32 z;
			};
			Float32 v[3];
			glm::vec3 glmVec3;
		};
		Vector3() :x(0), y(0), z(0){}
		template<typename U>
			requires IsCastable<U, Float32>
		Vector3(U _x, U _y, U _z) : x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)), z(static_cast<Float32>(_z)) {}

		template<typename X, typename Y, typename Z>
			requires IsCastable<X, Float32>&& IsCastable<Y, Float32>
		Vector3(X _x, Y _y, Z _z) : x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)), z(static_cast<Float32>(_z)) {}

		//Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z){}
	};
}

