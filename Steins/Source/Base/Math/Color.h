#pragma once

#include "TypeTraits.h"

namespace Steins
{
	class Color 
	{
	public:
		union
		{
			struct
			{
				float32 x;
				float32 y;
				float32 z;
				float32 w;
			};

			struct
			{
				float32 r;
				float32 g;
				float32 b;
				float32 a;
			};

			float32 color[4];
			DirectX::XMFLOAT4 XMFloat;
		};
		Color() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Color(float32 _x, float32 _y, float32 _z, float32 _w) : x(_x), y(_y), z(_z), w(_w) {}

		template<typename U>
			requires IsCastable<U, float32>
		Color(U _x, U _y, U _z, U _w)
			: r(static_cast<float32>(_x)), g(static_cast<float32>(_y)), b(static_cast<float32>(_z)), a(static_cast<float32>(_w)) {}

		//template<typename X, typename Y, typename Z>
		//	requires IsCastable<X, T>&& IsCastable<Y, T>&& IsCastable<Z, T>
		//Color(X _x, Y _y, Z _z)
		//	: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(1)) {}

		//template<typename X, typename Y, typename Z, typename W>
		//	requires IsCastable<X, T>&& IsCastable<Y, T>&& IsCastable<Z, T>&& IsCastable<W, T>
		//Color(X _x, Y _y, Z _z, W _w)
		//	: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w)) {}
	};



}

