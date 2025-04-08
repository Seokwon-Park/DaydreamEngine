#pragma once

#include "Vector4.h"

namespace Steins
{
	class Matrix4x4
	{
	public:
		union
		{
			Float32 matrix[4][4];
			DirectX::XMMATRIX dxMatrix;
		};

		Matrix4x4()
		{
			dxMatrix = DirectX::XMMatrixIdentity();
		}

		void MatrixInverse()
		{
			dxMatrix = DirectX::XMMatrixInverse(nullptr, dxMatrix);
		}

		void MatrixTranslation(Vector4 _translate)
		{
			DirectX::XMVECTOR Vec = DirectX::XMLoadFloat4A(&_translate.XMFloat);
			dxMatrix = DirectX::XMMatrixTranslationFromVector(Vec);
		}

		Matrix4x4 operator*(Matrix4x4 _matrix)
		{
			DirectX::XMMATRIX XMMat = DirectX::XMMatrixMultiply(dxMatrix, _matrix.dxMatrix);
			Matrix4x4 Result;
			Result.dxMatrix = XMMat;
			return Result;
		}

	};
}

