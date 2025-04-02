#pragma once

#include <DirectXMath.h>
#include "TypeTraits.h"

namespace Steins
{
	template<typename T>
		requires IsValid<T>
	class Vector4
	{

	public:
		union
		{
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};
			T v[4];
			DirectX::XMFLOAT4 XMFloat;
		};
		Vector4() :x(0), y(0), z(0), w(0) {}
		Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

		//template<typename U>
		//	requires IsCastable<U, T>
		//Vector4(U _x, U _y, U _z, U _w)
		//	: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w)) {}

		template<typename X, typename Y, typename Z>
			requires IsCastable<X, T>&& IsCastable<Y, T>&& IsCastable<Z, T>
		Vector4(X _x, Y _y, Z _z)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(1)) {}

		template<typename X, typename Y, typename Z, typename W>
			requires IsCastable<X, T>&& IsCastable<Y, T>&& IsCastable<Z, T>&& IsCastable<W, T>
		Vector4(X _x, Y _y, Z _z, W _w)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w)) {}

		static const Vector4 ZERO;
		static const Vector4 LEFT;
		static const Vector4 RIGHT;
		static const Vector4 UP;
		static const Vector4 DOWN;

		static friend std::ostream& operator<<(std::ostream& _ostream, const Vector4& _vector)
		{
			_ostream << "(" << _vector.x << ", " << _vector.y << ", " << _vector.z << ", " << _vector.w << ")";
			return _ostream;
		}

		Vector4 operator-() const { return Vector4(-x, -y); }
		Vector4 operator+(const Vector4& _other) const { return Vector4(x + _other.x, y + _other.y); }
		Vector4 operator-(const Vector4& _other) const;
		Vector4 operator/(const Vector4& _other) const;
		Vector4 operator*(const float& _value) const;
		Vector4 operator/(const float& _value) const;

		//operator FIntPoint();

		void operator+=(const Vector4& _other);
		void operator-=(const Vector4& _other);
		void operator*=(const float& _value);
		void operator*=(const Vector4& _other);
		void operator/=(const float& _value);
	};

	template <typename T> requires IsValid<T> const Vector4<T> Vector4<T>::ZERO = Vector4(0, 0);
	template <typename T> requires IsValid<T> const Vector4<T> Vector4<T>::LEFT = Vector4(-1, 0);
	template <typename T> requires IsValid<T> const Vector4<T> Vector4<T>::RIGHT = Vector4(1, 0);
	template <typename T> requires IsValid<T> const Vector4<T> Vector4<T>::UP = Vector4(0, 1);
	template <typename T> requires IsValid<T> const Vector4<T> Vector4<T>::DOWN = Vector4(0, -1);
}


