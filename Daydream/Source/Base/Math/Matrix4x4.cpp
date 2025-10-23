#include "DaydreamPCH.h"
#include "Matrix4x4.h"

#include "MathUtil.h"

#include "glm/gtx/matrix_decompose.hpp"

namespace Daydream
{
	Matrix4x4::Matrix4x4()
	{
		glmMat = glm::mat4(1.0f);
	}
	Matrix4x4& Matrix4x4::MatrixInverse()
	{
		glmMat = glm::inverse(glmMat);
		return *this;
	}

	Matrix4x4& Matrix4x4::MatrixTranslate(Vector3 _translate)
	{
		glmMat = glm::translate(glmMat, _translate);
		return *this;
	}
	Matrix4x4& Matrix4x4::MatrixTranslate(Vector4 _translate)
	{
		glmMat = glm::translate(glmMat, glm::vec3(_translate.x, _translate.y, _translate.z));
		return *this; 
	}
	Matrix4x4& Matrix4x4::MatrixRotate(Vector3 _rollPitchYaw)
	{
		Vector3 radians = glm::radians(_rollPitchYaw);
		Quaternion quat = glm::quat(radians);
		glmMat = glm::toMat4(quat);

		return *this;
	}
	Matrix4x4& Matrix4x4::MatrixTranspose()
	{
		glmMat = glm::transpose(glmMat);
		return *this;
	}

	Matrix4x4 Matrix4x4::GetInverse() const
	{
		Matrix4x4 result = *this; // 1. 나를 복사
		result.glmMat = glm::inverse(this->glmMat); // 2. 복사본을 수정
		return result; // 3. 복사본을 반환
	}

	Matrix4x4 Matrix4x4::GetTranslate(Vector3 _translate) const
	{
		Matrix4x4 result = *this;
		// [주의] glm::translate는 원본을 *수정하지 않고* 새 행렬을 반환합니다.
		// 따라서 'this->glmMat'을 기반으로 연산해야 합니다.
		result.glmMat = glm::translate(this->glmMat, _translate);
		return result;
	}

	Matrix4x4 Matrix4x4::GetRotate(Vector3 _rollPitchYaw) const
	{
		Matrix4x4 result = *this;
		glm::vec3 radians = glm::radians(_rollPitchYaw);
		glm::quat rotationQuat = glm::quat(radians);

		// 원본 'this->glmMat'에 회전을 곱한 결과를 'result.glmMat'에 저장
		result.glmMat = this->glmMat * glm::toMat4(rotationQuat);
		return result;
	}

	Matrix4x4 Matrix4x4::GetTranspose() const
	{
		Matrix4x4 result = *this;
		result.glmMat = glm::transpose(this->glmMat);
		return result;
	}

	Matrix4x4 Matrix4x4::operator*(Matrix4x4 _matrix)
	{
		Matrix4x4 result;
		result.glmMat =  glmMat * _matrix.glmMat;
		return result;
	}

	Vector4 Matrix4x4::operator*(Vector4 _vector) const
	{
		return glmMat * _vector;
	}

	Vector4 Matrix4x4::operator*(Vector3 _vector) const
	{
		return glmMat * Vector4(_vector, 1.0f);
	}

	Matrix4x4 Matrix4x4::CreateTranslation(Vector3 _translate)
	{
		return Translate(Matrix4x4(), _translate);
	}

	Matrix4x4 Matrix4x4::Translate(Matrix4x4 _matrix, Vector3 _translate)
	{
		_matrix.MatrixTranslate(_translate);
		return _matrix;
	}

	Matrix4x4 Matrix4x4::Transpose(Matrix4x4 _matrix)
	{
		_matrix.MatrixTranspose();
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
		out.glmMat = glm::perspectiveLH_ZO(_fovy, _aspect, _near, _far);
		return out;
	}

	Matrix4x4 Matrix4x4::Inverse(Matrix4x4 _matrix)
	{
		_matrix.MatrixInverse();
		return _matrix;
	}

	Matrix4x4 Matrix4x4::CreateLookTo(Vector3 _eye, Vector3 _direction, Vector3 _up)
	{
		Matrix4x4 out;
		out.glmMat = glm::lookAt(_eye, _eye + glm::normalize(_direction), _up);
		return out;
	}
	Matrix4x4 Matrix4x4::QuatToMatrix(Quaternion _quat)
	{
		Matrix4x4 mat;
		mat.glmMat = glm::toMat4(_quat);
		return mat;
	}
	Matrix4x4 Matrix4x4::TransformToWorldMatrix(Transform _transform)
	{
		Matrix4x4 world = Matrix4x4();
		world.glmMat = glm::translate(world.glmMat, _transform.position);
		world.glmMat *= QuatToMatrix(Quaternion(glm::radians(_transform.rotation))).glmMat;
		world.glmMat = glm::scale(world.glmMat, _transform.scale);
		return world;
	}

	bool Matrix4x4::Decompose(const Matrix4x4& _matrix, Vector3& _outTranslation, Vector3& _outRotation, Vector3& _outScale)
	{
		Quaternion rotation;
		Vector3 skew;
		Vector4 perspective;
		glm::decompose(_matrix.glmMat, _outScale, rotation, _outTranslation, skew, perspective);

		_outRotation = glm::degrees(glm::eulerAngles(rotation));
		
		return false;
	}
}
