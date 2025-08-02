#pragma once

#include "TypeTraits.h"

namespace Daydream
{
	class Color 
	{
	public:
		union
		{
			struct
			{
				Float32 x;
				Float32 y;
				Float32 z;
				Float32 w;
			};

			struct
			{
				Float32 r;
				Float32 g;
				Float32 b;
				Float32 a;
			};

			Float32 color[4];
		};
		Color() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Color(Float32 _x, Float32 _y, Float32 _z, Float32 _w) : x(_x), y(_y), z(_z), w(_w) {}

		template<typename U>
			requires IsCastable<U, Float32>
		Color(U _x, U _y, U _z, U _w)
			: r(static_cast<Float32>(_x)), g(static_cast<Float32>(_y)), b(static_cast<Float32>(_z)), a(static_cast<Float32>(_w)) {}

		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
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

