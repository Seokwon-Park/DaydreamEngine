#pragma once

#include "Base/MathTypes.h"
#include "Base/Math/Matrix4x4.h"

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

		inline static Vector3 Min(const Vector3& a, const Vector3& b)
		{
			return glm::min(a, b); // x는 x끼리, y는 y끼리 비교
		}

		// AABB 계산용 Max (Component-wise)
		inline static Vector3 Max(const Vector3& a, const Vector3& b)
		{
			return glm::max(a, b);
		}
	};
}