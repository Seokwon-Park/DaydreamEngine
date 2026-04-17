#pragma once

#include "TypeTraits.h"

namespace Daydream
{
	struct Color
	{
		union
		{
			struct { Float32 r, g, b, a; };
			Float32 color[4];
			Vector4 vec;
		};

		Color() :r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
		Color(Float32 _r, Float32 _g, Float32 _b, Float32 _a) : r(_r), g(_g), b(_b), a(_a) {}

		template<typename U>
			requires IsCastable<U, Float32>
		Color(U _r, U _g, U _b, U _a)
			: r(static_cast<Float32>(_r)), g(static_cast<Float32>(_g)), b(static_cast<Float32>(_b)), a(static_cast<Float32>(_a)) {
		}

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

