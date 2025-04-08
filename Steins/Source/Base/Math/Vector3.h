#pragma once

namespace Steins
{
	template<typename T>
		requires IsValid<T>
	class Vector3
	{
	public:
		union
		{
			struct
			{
				T x;
				T y;
				T z;
			};
			T v[3];
			DirectX::XMFLOAT3 XMFloat;
		};
		Vector3() :x(0), y(0), z(0){}
		Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z){}
	};
}

