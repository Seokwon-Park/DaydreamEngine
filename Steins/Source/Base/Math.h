#pragma once

#include "Base/MathTypes.h"
#include "Base/Math/Matrix4x4.h"

namespace Steins
{
	namespace Math
	{


		inline static Matrix4x4 Inverse(Matrix4x4 _matrix)
		{
			Matrix4x4 mat;
			//mat.dxMatrix = DirectX::XMMatrixInverse(nullptr, _matrix.dxMatrix);
			mat.glmMatrix = glm::inverse(_matrix.glmMatrix);
			return mat;
		}

		inline static Matrix4x4 Orthographic(float _left, float _right, float _bottom, float _top)
		{
			Matrix4x4 mat;
			mat.dxMatrix = DirectX::XMMatrixOrthographicOffCenterLH(_left, _right, _bottom, _top, 0.0f, 1.0f);
			//mat.glmMatrix = glm::m(mat.glmMatrix);

			return mat;
		}
	}
}