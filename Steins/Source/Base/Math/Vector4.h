#pragma once

#include <iostream>
#include "TypeTraits.h"
#include "Vector2.h"

#include "glm/glm.hpp"


namespace Steins
{
	using Vector4 = glm::vec4;
	//class Vector4
	//{

	//public:
	//	union
	//	{
	//		struct
	//		{
	//			Float32 x;
	//			Float32 y;
	//			Float32 z;
	//			Float32 w;
	//		};
	//		Float32 v[4];
	//	};

	//	constexpr Vector4() :x(0), y(0), z(0), w(0) {}
	//	constexpr Vector4(Vector2 _vec2, Float32 _z, Float32 _w);
	//	constexpr Vector4(Float32 _x, Float32 _y, Float32 _z, Float32 _w) : x(_x), y(_y), z(_z), w(_w) {}

	//	//template<typename U>
	//	//	requires IsCastable<U, Float32>
	//	//Vector4(U _x, U _y, U _z, U _w)
	//	//	: x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)), z(static_cast<Float32>(_z)), w(static_cast<Float32>(_w)) {}

	//	template<typename X, typename Y, typename Z>
	//		requires IsCastable<X, Float32>&& IsCastable<Y, Float32>&& IsCastable<Z, Float32>
	//	Vector4(X _x, Y _y, Z _z)
	//		: x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)), z(static_cast<Float32>(_z)), w(1.0f) {}

	//	template<typename X, typename Y, typename Z, typename W>
	//		requires IsCastable<X, Float32>&& IsCastable<Y, Float32>&& IsCastable<Z, Float32>&& IsCastable<W, Float32>
	//	Vector4(X _x, Y _y, Z _z, W _w)
	//		: x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)), z(static_cast<Float32>(_z)), w(static_cast<Float32>(_w)) {}

	//	static const Vector4 ZERO;
	//	static const Vector4 LEFT;
	//	static const Vector4 RIGHT;
	//	static const Vector4 UP;
	//	static const Vector4 DOWN;

	//	static friend std::ostream& operator<<(std::ostream& _ostream, const Vector4& _vector)
	//	{
	//		_ostream << "(" << _vector.x << ", " << _vector.y << ", " << _vector.z << ", " << _vector.w << ")";
	//		return _ostream;
	//	}

	//	Vector4 operator-() const;
	//	Vector4 operator+(const Vector4& _other) const;
	//	Vector4 operator-(const Vector4& _other) const;
	//	Vector4 operator/(const Vector4& _other) const;
	//	Vector4 operator*(const Float32& _value) const;
	//	Vector4 operator/(const Float32& _value) const;

	//	void operator+=(const Vector4& _other);
	//	void operator-=(const Vector4& _other);
	//	void operator*=(const Float32& _value);
	//	void operator*=(const Vector4& _other);
	//	void operator/=(const Float32& _value);
	//};

	/*const Vector4 Vector4::ZERO = Vector4(0, 0, 0, 0);
	const Vector4 Vector4::LEFT = Vector4(-1, 0, 0, 0);
	const Vector4 Vector4::RIGHT = Vector4(1, 0);
	const Vector4 Vector4::UP = Vector4(0, 1);
	const Vector4 Vector4::DOWN = Vector4(0, -1);*/
}


