#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

#include "glm/glm.hpp"

namespace Daydream
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
		void MatrixRotate(Vector3 _rollPitchYaw);

		void MatrixTranspose();

		Matrix4x4 operator*(Matrix4x4 _matrix);
		Vector4 operator*(Vector3 _vector) const;
		Vector4 operator*(Vector4 _vector) const;

		static Matrix4x4 Translate(Vector3 _translate);
		static Matrix4x4 Translate(Matrix4x4 _matrix, Vector3 _translate);
		static Matrix4x4 Rotate(Matrix4x4 _matrix, Vector3 _rollPitchYaw);
		static Matrix4x4 Orthographic(Float32 _size, Float32 _aspectRatio,Float32 _near, Float32 _far);
		static Matrix4x4 Orthographic(Float32 _left, Float32 _right, Float32 _bottom, Float32 _top, Float32 _near, Float32 _far);
		static Matrix4x4 Perspective(Float32 _fovy, Float32 _aspect, Float32 _near, Float32 _far);
		static Matrix4x4 Inverse(Matrix4x4 _matrix);
		static Matrix4x4 LookTo(Vector3 _eye, Vector3 _direction, Vector3 _up);

		static Matrix4x4 QuatToMatrix(Quaternion _quat);



	};
}

