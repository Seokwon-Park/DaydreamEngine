#include "SteinsPCH.h"
#include "Vector2.h"

namespace Steins
{
	Vector2 Vector2::operator+(const Vector2& _other) const
	{
		return Vector2(x + _other.x, y + _other.y);
	}

	Vector2 Vector2::operator-() const
	{
		return Vector2(-x, -y);
	}

	Vector2 Vector2::operator-(const Vector2& _other) const
	{
		return Vector2(x - _other.x, y - _other.y);
	}

	Vector2 Vector2::operator/(const Vector2& _other) const
	{
		if (_other.x == 0.0f || _other.y == 0.0f)
		{
			STEINS_CORE_ASSERT(false, "Can't divide by zero!");
			return Vector2();
		}
		return Vector2(x / _other.x, y / _other.y);

	}

	Vector2 Vector2::operator*(Float32 _scalar) const
	{
		return Vector2(x * _scalar, y * _scalar);
	}

	Vector2 Vector2::operator/(Float32 _scalar) const
	{
		if (_scalar == 0.0f)
		{
			STEINS_CORE_ASSERT(false, "Can't divide by zero!");
			return Vector2();
		}
		return Vector2(x / _scalar, y / _scalar);
	}

	void Vector2::operator+=(const Vector2& _other)
	{
		*this = *this + _other;
	}

	void Vector2::operator-=(const Vector2& _other)
	{
		*this = *this - _other;
	}


	void Vector2::operator*=(const Vector2& _other)
	{
		x *= _other.x;
		y *= _other.y;
	}

	void Vector2::operator*=(Float32 _scalar)
	{
		*this = *this * _scalar;
	}

	void Vector2::operator/=(Float32 _scalar)
	{
		*this *= 1.0f / _scalar;
	}
}
