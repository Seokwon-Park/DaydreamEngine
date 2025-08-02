#pragma once

#include <iostream>
#include "TypeTraits.h"

#include "glm/glm.hpp"


namespace Daydream
{
	using Vector2 = glm::vec2;
	//class Vector2
	//{

	//public:
	//	union
	//	{
	//		struct
	//		{
	//			Float32 x;
	//			Float32 y;
	//		};
	//		Float32 v[2];
	//	};
	//	Vector2() :x(0.0f), y(0.0f) {}
	//	Vector2(Float32 _x, Float32 _y) : x(_x), y(_y) {}

	//	template<typename U>
	//		requires IsCastable<U, Float32>
	//	Vector2(U _x, U _y) : x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)) {}

	//	template<typename X, typename Y>
	//		requires IsCastable<X, Float32> && IsCastable<Y, Float32>
	//	Vector2(X _x, Y _y) : x(static_cast<Float32>(_x)), y(static_cast<Float32>(_y)) {}

	//	static const Vector2 ZERO;
	//	static const Vector2 LEFT;
	//	static const Vector2 RIGHT;
	//	static const Vector2 UP;
	//	static const Vector2 DOWN;

	//	static friend std::ostream& operator<<(std::ostream& _ostream, const Vector2& _vector)
	//	{
	//		_ostream << "(" << _vector.x << ", " << _vector.y << ")";
	//		return _ostream;
	//	}

	//	Vector2 operator+(const Vector2& _other) const;
	//	Vector2 operator-() const;
	//	Vector2 operator-(const Vector2& _other) const;
	//	Vector2 operator/(const Vector2& _other) const;
	//	Vector2 operator*(Float32 _value) const;
	//	Vector2 operator/(Float32 _value) const;

	//	void operator+=(const Vector2& _other);
	//	void operator-=(const Vector2& _other);
	//	void operator*=(const Vector2& _other);
	//	void operator*=(Float32 _scalar);
	//	void operator/=(Float32 _scalar);

	//	bool operator==(const Vector2& other) const;
	//	bool operator!=(const Vector2& other) const;

	//	float Length() const;
	//	float LengthSquared() const;
	//	void Normalize();
	//	Vector2 Normalized() const; // 정규화된 새 벡터 반환

	//	static float Dot(const Vector2& a, const Vector2& b);
	//};

	//template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::ZERO = Vector2(0, 0);
	//template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::LEFT = Vector2(-1, 0);
	//template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::RIGHT = Vector2(1, 0);
	//template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::UP = Vector2(0, 1);
	//template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::DOWN = Vector2(0, -1);
}


