#include "DaydreamPCH.h"
#include "Matrix4x4.h"

#include "MathUtil.h"



namespace Daydream
{
	Matrix4x4::Matrix4x4()
	{
		glmMat = glm::mat4(1.0f);
	}
	Matrix4x4& Matrix4x4::Invert()
	{
		glmMat = glm::inverse(glmMat);
		return *this;
	}

	Matrix4x4& Matrix4x4::Translate(Vector3 _translate)
	{
		glmMat = glm::translate(glmMat, _translate);
		return *this;
	}

	Matrix4x4& Matrix4x4::Rotate(Vector3 _eulerAnlges)
	{
		Vector3 radians = glm::radians(_eulerAnlges);
		Quaternion quat = glm::quat(radians);
		glmMat = glm::toMat4(quat);

		return *this;
	}
	Matrix4x4& Matrix4x4::Transpose()
	{
		glmMat = glm::transpose(glmMat);
		return *this;
	}

	Matrix4x4 Matrix4x4::GetInversed() const
	{
		Matrix4x4 out = *this;
		out.glmMat = glm::inverse(this->glmMat);
		return out;
	}

	Matrix4x4 Matrix4x4::GetTranslated(Vector3 _translate) const
	{
		Matrix4x4 out = *this;
		out.glmMat = glm::translate(this->glmMat, _translate);
		return out;
	}

	Matrix4x4 Matrix4x4::GetRotated(Vector3 _rollPitchYaw) const
	{
		Matrix4x4 out = *this;
		Vector3 radians = glm::radians(_rollPitchYaw);
		Quaternion rotationQuat = Quaternion(radians);

		// 원본 'this->glmMat'에 회전을 곱한 결과를 'out.glmMat'에 저장
		out.glmMat = this->glmMat * glm::mat4_cast(rotationQuat);
		return out;
	}

	Matrix4x4 Matrix4x4::GetTranspose() const
	{
		Matrix4x4 out = *this;
		out.glmMat = glm::transpose(this->glmMat);
		return out;
	}

	//Matrix4x4 Matrix4x4::operator*(Matrix4x4 _matrix)
	//{
	//	Matrix4x4 out;
	//	out.glmMat =  glmMat * _matrix.glmMat;
	//	return out;
	//}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& _matrix) const
	{
		// GLM is Column-Major, but the Engine is Row-Major.
		// We reverse the multiplication order to handle this mismatch.
		Matrix4x4 out;
		out.glmMat = _matrix.glmMat * glmMat;
		return out;
	}

	//Vector4 Matrix4x4::operator*(Vector4 _vector) const
	//{
	//	return glmMat * _vector;
	//}

	//Vector4 Matrix4x4::operator*(Vector3 _vector) const
	//{
	//	return glmMat * Vector4(_vector, 1.0f);
	//}

	Matrix4x4 Matrix4x4::CreateTranslation(Vector3 _translation)
	{
		return CreateTranslation(Matrix4x4(), _translation);
	}

	Matrix4x4 Matrix4x4::CreateTranslation(const Matrix4x4& _matrix, Vector3 _translation)
	{
		Matrix4x4 out = _matrix;
		out.Translate(_translation);
		return out;
	}

	Matrix4x4 Matrix4x4::CreateRotation(Quaternion _quat)
	{
		Matrix4x4 mat;
		mat.glmMat = glm::mat4_cast(_quat);
		return mat;
	}

	Matrix4x4 Matrix4x4::CreateRotation(const Matrix4x4& _matrix, Vector3 _eulerAngles)
	{
		Matrix4x4 out = _matrix;
		out.Rotate(_eulerAngles);
		return out;
	}

	Matrix4x4 Matrix4x4::CreateTranspose(const Matrix4x4& _matrix)
	{
		Matrix4x4 out = _matrix;
		out.Transpose();
		return out;
	}

	Matrix4x4 Matrix4x4::CreateOrthographic(Float32 _size, Float32 _aspectRatio, Float32 _near, Float32 _far)
	{
		Matrix4x4 out;
		Float32 width = _size * _aspectRatio;
		return CreateOrthographic(-width / 2, width / 2, -_size / 2, _size / 2, _near, _far);
	}
	Matrix4x4 Matrix4x4::CreateOrthographic(Float32 _left, Float32 _right, Float32 _bottom, Float32 _top, Float32 _near, Float32 _far)
	{
		Matrix4x4 out;
		out.glmMat = glm::ortho(_left, _right, _bottom, _top, _near, _far);
		return out;

	}
	Matrix4x4 Matrix4x4::CreatePerspective(Float32 _fovy, Float32 _aspect, Float32 _near, Float32 _far)
	{
		Matrix4x4 out;
		out.glmMat = glm::perspectiveLH_ZO(_fovy, _aspect, _near, _far);
		return out;
	}

	Matrix4x4 Matrix4x4::CreateInverse(const Matrix4x4& _matrix)
	{
		Matrix4x4 out;
		out.Invert();
		return out;
	}

	Matrix4x4 Matrix4x4::CreateLookTo(Vector3 _eye, Vector3 _direction, Vector3 _up)
	{
		Matrix4x4 out;
		out.glmMat = glm::lookAt(_eye, _eye + glm::normalize(_direction), _up);
		return out;
	}

	Vector3 Matrix4x4::TransformPoint(const Matrix4x4& _mat, const Vector3& _point)
	{
		Vector4 res = _mat.glmMat * Vector4(_point.x, _point.y, _point.z, 1.0f);
		return Vector3(res.x, res.y, res.z);
	}

	Vector3 Matrix4x4::TransformDirection(const Matrix4x4& _mat, const Vector3& _dir)
	{
		Vector4 res = _mat.glmMat * Vector4(_dir.x, _dir.y, _dir.z, 0.0f);
		return Vector3(res.x, res.y, res.z);
	}


}
