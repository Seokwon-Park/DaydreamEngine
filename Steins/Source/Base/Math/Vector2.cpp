#include "SteinsPCH.h"
#include "Vector2.h"

namespace Steins
{
	const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::LEFT = Vector2(-1.0f, 0.0f);
	const Vector2 Vector2::RIGHT = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::UP = Vector2(0.0f, 1.0f);
	const Vector2 Vector2::DOWN = Vector2(0.0f, -1.0f);


	Vector2::Vector2()
		:X(0.0f), Y(0.0f)
	{

	}
	Vector2::Vector2(int _X, int _Y)
		:X(static_cast<float>(_X)), Y(static_cast<float>(_Y))
	{

	}
	Vector2::Vector2(float _X, float _Y)
		:X(_X), Y(_Y)
	{

	}

	Vector2::Vector2(double _X, double _Y)
		:X(static_cast<float>(_X)), Y(static_cast<float>(_Y))
	{
	}

	Vector2 Vector2::operator+(const Vector2& _Other) const
	{
		return Vector2(X + _Other.X, Y + _Other.Y);
	}

	Vector2 Vector2::operator-() const
	{
		return Vector2(-X, -Y);
	}

	Vector2 Vector2::operator-(const Vector2& _Other) const
	{
		return Vector2(X - _Other.X, Y - _Other.Y);
	}

	Vector2 Vector2::operator/(const Vector2& _Other) const
	{
		return Vector2(X / _Other.X, Y / _Other.Y);
	}

	Vector2 Vector2::operator*(const float& _Value) const
	{
		return Vector2(X * _Value, Y * _Value);
	}

	Vector2 Vector2::operator/(const float& _Value) const
	{
		return Vector2(X / _Value, Y / _Value);
	}

	//Vector2::operator FIntPoint()
	//{
	//	return FIntPoint(X, Y);
	//}

	void Vector2::operator+=(const Vector2& _Other)
	{
		*this = *this + _Other;
	}

	void Vector2::operator-=(const Vector2& _Other)
	{
		*this = *this - _Other;
	}

	void Vector2::operator*=(const float& _Value)
	{
		*this = *this * _Value;
	}

	void Vector2::operator*=(const Vector2& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
	}

	void Vector2::operator/=(const float& _Value)
	{
		*this *= 1.0f / _Value;
	}
}
