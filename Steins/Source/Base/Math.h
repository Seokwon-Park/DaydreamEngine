#pragma once

#include "Base/MathTypes.h"

namespace Steins
{
	namespace Math
	{
		inline static Matrix4x4 Translate(Matrix4x4 _matrix, Vector4 _translate)
		{
			DirectX::XMVECTOR vec = DirectX::XMLoadFloat4A(&_translate.XMFloat);
			Matrix4x4 mat;
			mat.dxMatrix = DirectX::XMMatrixTranslationFromVector(vec);
			return mat;
		}

		inline static Matrix4x4 Inverse(Matrix4x4 _matrix)
		{
			Matrix4x4 mat;
			mat.dxMatrix = DirectX::XMMatrixInverse(nullptr, _matrix.dxMatrix);
			return mat;
		}

		inline static Matrix4x4 Ortho(float _left, float _right, float _bottom, float _top)
		{
			Matrix4x4 mat;
			mat.dxMatrix = DirectX::XMMatrixOrthographicOffCenterLH(_left, _right, _bottom, _top, 0.0f, 1.0f);
			return mat;
		}
	}
}