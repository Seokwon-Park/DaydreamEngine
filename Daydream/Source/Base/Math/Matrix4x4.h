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
		Matrix4x4& Invert();
		Matrix4x4& Translate(Vector3 _translate);
		Matrix4x4& Rotate(Vector3 _eulerAnlges);
		Matrix4x4& Transpose();

		[[nodiscard]]
		Matrix4x4 GetInversed() const;
		[[nodiscard]]
		Matrix4x4 GetTranslated(Vector3 _translate) const;
		[[nodiscard]]
		Matrix4x4 GetRotated(Vector3 _eulerAnlges) const;
		[[nodiscard]]
		Matrix4x4 GetTranspose() const;

		Matrix4x4 operator*(const Matrix4x4& _matrix) const;
		//Vector4 operator*(Vector3 _vector) const;
		//Vector4 operator*(Vector4 _vector) const;

		static Matrix4x4 CreateTranslation(Vector3 _translation);
		static Matrix4x4 CreateTranslation(const Matrix4x4& _matrix, Vector3 _translate);
		static Matrix4x4 CreateRotation(Quaternion _quat);
		static Matrix4x4 CreateRotation(const Matrix4x4& _matrix, Vector3 _eulerAnlges);
		static Matrix4x4 CreateTranspose(const Matrix4x4& _matrix);
		static Matrix4x4 CreateOrthographic(Float32 _size, Float32 _aspectRatio, Float32 _near, Float32 _far);
		static Matrix4x4 CreateOrthographic(Float32 _left, Float32 _right, Float32 _bottom, Float32 _top, Float32 _near, Float32 _far);
		static Matrix4x4 CreatePerspective(Float32 _fovy, Float32 _aspect, Float32 _near, Float32 _far);
		static Matrix4x4 CreateInverse(const Matrix4x4& _matrix);
		static Matrix4x4 CreateLookTo(Vector3 _eye, Vector3 _direction, Vector3 _up);

		static Vector3 TransformPoint(const Matrix4x4& _mat, const Vector3& _point);
		static Vector3 TransformDirection(const Matrix4x4& _mat, const Vector3& _dir);
	};
}

