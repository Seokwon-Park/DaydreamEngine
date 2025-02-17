#pragma once

#include <DirectXMath.h>

namespace Steins
{
	class Vector2
	{
	public:
		union
		{
			struct
			{
				float X;
				float Y;
			};
			float V[2];
			DirectX::XMFLOAT2 XMFloat;
		};

		static const Vector2 ZERO;
		static const Vector2 LEFT;
		static const Vector2 RIGHT;
		static const Vector2 UP;
		static const Vector2 DOWN;

		Vector2();
		Vector2(int _X, int _Y);
		Vector2(float _X, float _Y);
		Vector2(double _X, double _Y);

		static friend std::ostream& operator<<(std::ostream& _Ostream, const Vector2& _Vector)
		{
			_Ostream << "(" << _Vector.X << ", " << _Vector.Y << ")";
			return _Ostream;
		}

		Vector2 operator+(const Vector2& _Other) const;
		Vector2 operator-() const;
		Vector2 operator-(const Vector2& _Other) const;
		Vector2 operator/(const Vector2& _Other) const;
		Vector2 operator*(const float& _Value) const;
		Vector2 operator/(const float& _Value) const;

		//operator FIntPoint();

		void operator+=(const Vector2& _Other);
		void operator-=(const Vector2& _Other);
		void operator*=(const float& _Value);
		void operator*=(const Vector2& _Other);
		void operator/=(const float& _Value);
	};
}


