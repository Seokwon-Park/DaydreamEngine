#pragma once

#include "Base/MathTypes.h"
#include "Base/Math/Matrix4x4.h"

namespace Daydream
{
	namespace Math
	{
		inline Float32 DegreeToRadian(Float32 _degrees)
		{
			return glm::radians(_degrees);
		}

		inline Float32 RadianToDegree(Float32 _radians)
		{
			return glm::degrees(_radians);
		}

		inline Float32 Cos(Float32 _radian)
		{
			return cosf(_radian);
		}

		inline Float32 CosDegree(Float32 _degrees)
		{
			return cosf(DegreeToRadian(_degrees));
		}
	};
}