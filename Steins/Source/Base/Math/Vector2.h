#pragma once

#include <DirectXMath.h>
#include "TypeTraits.h"

namespace Steins
{
	template<typename T>
		requires IsValid<T>
	class Vector2
	{

	public:
		union
		{
			struct
			{
				T x;
				T y;
			};
			T v[2];
			DirectX::XMFLOAT2 XMFloat;
		};
		Vector2() :x(0), y(0) {}
		Vector2(T _x, T _y) : x(_x), y(_y) {}

		//template<typename U>
		//	requires IsCastable<U, T>
		//Vector2(U _x, U _y) : x(static_cast<T>(_x)), y(static_cast<T>(_y)) {}

		template<typename X, typename Y>
			requires IsCastable<X, T> && IsCastable<Y, T>
		Vector2(X _x, Y _y) : x(static_cast<T>(_x)), y(static_cast<T>(_y)) {}

		static const Vector2 ZERO;
		static const Vector2 LEFT;
		static const Vector2 RIGHT;
		static const Vector2 UP;
		static const Vector2 DOWN;

		static friend std::ostream& operator<<(std::ostream& _ostream, const Vector2& _vector)
		{
			_ostream << "(" << _vector.x << ", " << _vector.y << ")";
			return _ostream;
		}

		Vector2 operator+(const Vector2& _other) const { return Vector2(x + _other.x, y + _other.y); }
		Vector2 operator-() const { return Vector2(-x, -y); }
		Vector2 operator-(const Vector2& _other) const;
		Vector2 operator/(const Vector2& _other) const;
		Vector2 operator*(const float& _value) const;
		Vector2 operator/(const float& _value) const;

		//operator FIntPoint();

		void operator+=(const Vector2& _other);
		void operator-=(const Vector2& _other);
		void operator*=(const float& _value);
		void operator*=(const Vector2& _other);
		void operator/=(const float& _value);
	};

	template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::ZERO = Vector2(0, 0);
	template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::LEFT = Vector2(-1, 0);
	template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::RIGHT = Vector2(1, 0);
	template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::UP = Vector2(0, 1);
	template <typename T> requires IsValid<T> const Vector2<T> Vector2<T>::DOWN = Vector2(0, -1);
}


