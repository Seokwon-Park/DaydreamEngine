#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Transform.h"

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

		Matrix4x4& MatrixInverse();
		Matrix4x4& MatrixTranslate(Vector3 _translate);
		Matrix4x4& MatrixTranslate(Vector4 _translate);
		Matrix4x4& MatrixRotate(Vector3 _rollPitchYaw);
		Matrix4x4& MatrixTranspose();


		Matrix4x4 GetInverse() const;
		Matrix4x4 GetTranslate(Vector3 _translate) const;
		Matrix4x4 GetTranslate(Vector4 _translate) const;
		Matrix4x4 GetRotate(Vector3 _rollPitchYaw) const;
		Matrix4x4 GetTranspose() const;

		Matrix4x4 operator*(Matrix4x4 _matrix);
		Vector4 operator*(Vector3 _vector) const;
		Vector4 operator*(Vector4 _vector) const;

		static Matrix4x4 CreateTranslation(Vector3 _translate);
		static Matrix4x4 Translate(Matrix4x4 _matrix, Vector3 _translate);
		static Matrix4x4 Transpose(Matrix4x4 _matrix);
		static Matrix4x4 Rotate(Matrix4x4 _matrix, Vector3 _rollPitchYaw);
		static Matrix4x4 Orthographic(Float32 _size, Float32 _aspectRatio, Float32 _near, Float32 _far);
		static Matrix4x4 Orthographic(Float32 _left, Float32 _right, Float32 _bottom, Float32 _top, Float32 _near, Float32 _far);
		static Matrix4x4 Perspective(Float32 _fovy, Float32 _aspect, Float32 _near, Float32 _far);
		static Matrix4x4 Inverse(Matrix4x4 _matrix);
		static Matrix4x4 CreateLookTo(Vector3 _eye, Vector3 _direction, Vector3 _up);

		static Matrix4x4 QuatToMatrix(Quaternion _quat);
		static Matrix4x4 TransformToWorldMatrix(Transform _transform);

		static bool Decompose(const Matrix4x4& matrix,
			Vector3& outTranslation,
			Vector3& outRotation, // 오일러(Vector3)보다 쿼터니언이 안정적입니다.
			Vector3& outScale);



	};
}

