#pragma once

#include "Base/MathTypes.h"
#include "Base/Math/Matrix4x4.h"

namespace Daydream
{
	class Math
	{
		inline static Float32 DegToRad(Float32 _degrees)
		{
			return glm::radians(_degrees);
		}

		inline static Float32 RadToDeg(Float32 _radians)
		{
			return glm::degrees(_radians);
		}
	};
}