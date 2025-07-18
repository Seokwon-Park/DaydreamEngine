#include "SteinsPCH.h"
#include "Matrix4x4.h"

#include "MathUtil.h"

namespace Steins
{
	Matrix4x4::Matrix4x4()
	{
		glmMat = glm::mat4(1.0f);
	}
	void Matrix4x4::MatrixInverse()
	{
		glmMat = glm::inverse(glmMat);
	}
	void Matrix4x4::MatrixTranslate(Vector3 _translate)
	{
		glmMat = glm::translate(glmMat, _translate);
	}
	void Matrix4x4::MatrixTranslate(Vector4 _translate)
	{

	}
	void Matrix4x4::MatrixRotate(Vector3 _rollPitchYaw)
	{
	}
	void Matrix4x4::MatrixTranspose()
	{
		glmMat = glm::transpose(glmMat);
	}

	Matrix4x4 Matrix4x4::operator*(Matrix4x4 _matrix)
	{
		glmMat *= _matrix.glmMat;
		return *this;
	}

	Vector4 Matrix4x4::operator*(Vector4 _vector) const
	{
		return glmMat * _vector;
	}

	Matrix4x4 Matrix4x4::Translate(Vector3 _translate)
	{
		return Translate(Matrix4x4(), _translate);
	}

	Matrix4x4 Matrix4x4::Translate(Matrix4x4 _matrix, Vector3 _translate)
	{
		_matrix.MatrixTranslate(_translate);
		return _matrix;
	}

	Matrix4x4 Matrix4x4::Rotate(Matrix4x4 _matrix, Vector3 _rollPitchYaw)
	{
		_matrix.MatrixRotate(_rollPitchYaw);
		return Matrix4x4();
	}
	Matrix4x4 Matrix4x4::Orthographic(Float32 _size, Float32 _aspectRatio, Float32 _near, Float32 _far)
	{
		Matrix4x4 out;
		Float32 width = _size * _aspectRatio;
		return Orthographic(-width / 2, width / 2, -_size / 2, _size / 2, _near, _far);
	}
	Matrix4x4 Matrix4x4::Orthographic(Float32 _left, Float32 _right, Float32 _bottom, Float32 _top, Float32 _near, Float32 _far)
	{
		Matrix4x4 out;
		out.glmMat = glm::ortho(_left, _right, _bottom, _top, _near, _far);
		return out;

	}
	Matrix4x4 Matrix4x4::Perspective(Float32 _fovy, Float32 _aspect, Float32 _near, Float32 _far)
	{
		Matrix4x4 out;
		out.glmMat = glm::perspectiveRH_NO(_fovy, _aspect, _near, _far);
		out.glmMat = glm::perspectiveLH_ZO(_fovy, _aspect, _near, _far);
		return out;
	}

	Matrix4x4 Matrix4x4::Inverse(Matrix4x4 _matrix)
	{
		_matrix.MatrixInverse();
		return _matrix;
	}

	Matrix4x4 Matrix4x4::LookTo(Vector3 _eye, Vector3 _direction, Vector3 _up)
	{
		Matrix4x4 out;
		out.glmMat = glm::lookAt(_eye, _eye + glm::normalize(_direction), _up);
		return out;
	}
}
