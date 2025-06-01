#pragma once

#include "Vector3.h"
#include "Vector4.h"

#include "glm/glm.hpp"

namespace Steins
{
	class Matrix4x4
	{
	public:
		union
		{
			Float32 mat[4][4];
			Float32 values[16];
			glm::mat4 glmMat;
		};

		Matrix4x4();

		void MatrixInverse();

		void MatrixTranslate(Vector3 _translate);
		void MatrixTranslate(Vector4 _translate);

		void MatrixTranspose();

		Matrix4x4 operator*(Matrix4x4 _matrix);
		Vector4 operator*(Vector4 _vector) const;

		static Matrix4x4 Translate(Matrix4x4 _matrix, Vector3 _translate);
		static Matrix4x4 Orthographic(float _left, float _right, float _bottom, float _top);
		static Matrix4x4 Inverse(Matrix4x4 _matrix);


	};
}

