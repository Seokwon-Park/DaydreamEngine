#pragma once

#include "Base/MathTypes.h"
#include "Base/Math/Matrix4x4.h"

#include <bit>

namespace Daydream
{
	class Math
	{
	public:
		inline static Float32 DegreeToRadian(Float32 _degrees)
		{
			return glm::radians(_degrees);
		}

		inline static Float32 RadianToDegree(Float32 _radians)
		{
			return glm::degrees(_radians);
		}

		inline static Float32 Cos(Float32 _radian)
		{
			return cosf(_radian);
		}

		inline static Float32 CosDegree(Float32 _degrees)
		{
			return cosf(DegreeToRadian(_degrees));
		}

		template <typename T>
		inline static T Min(const T& _a, const T& _b)
		{
			return glm::min(_a, _b);
		}

		template <typename T>
		inline static T Max(const T& _a, const T& _b)
		{
			return glm::max(_a, _b);
		}

		// 필요하다면 Clamp도 똑같이 처리 가능
		template <typename T>
		inline static T Clamp(const T& _val, const T& _min, const T& _max)
		{
			return glm::clamp(_val, _min, _max);
		}

		template <std::unsigned_integral T>
		inline static T BitCount(T _x)
		{
			return std::popcount(_x);
		}

	};
}