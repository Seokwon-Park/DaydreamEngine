#include "DaydreamPCH.h"
#include "Transform.h"

#include "Matrix4x4.h"
#include "glm/gtx/matrix_decompose.hpp"

namespace Daydream
{
	Transform::Transform()
	{
	}
	Transform::Transform(const Vector3& _position)
	{
		position = _position;
	}
	Transform::Transform(const Vector3& _position, const Vector3& _rotation)
	{
	}
	Transform::Transform(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale)
	{
	}

	//Vector3 Transform::GetForward()
	//{
	//	return glm::rotate(Quaternion(glm::radians(rotation)), Vector3(0.0f,0.0f,1.0f));
	//}
	//Vector3 Transform::GetUp()
	//{
	//	return glm::rotate(Quaternion(glm::radians(rotation)), Vector3(0.0f, 1.0f, 0.0f));
	//}
	//Vector3 Transform::GetRight()
	//{
	//	return glm::cross(GetUp(), GetForward());
	//}

	Vector3 Transform::GetForward() const
	{
		return glm::rotate(Quaternion(glm::radians(rotation)), Vector3(0.0f, 0.0f, 1.0f));
	}

	Vector3 Transform::GetUp() const
	{
		return glm::rotate(Quaternion(glm::radians(rotation)), Vector3(0.0f, 1.0f, 0.0f));
	}

	Vector3 Transform::GetRight() const
	{
		return glm::cross(GetUp(), GetForward());
	}

	Quaternion Transform::GetOrientation()
	{
		return Quaternion(rotation);
	}

	Matrix4x4 Transform::CreateLocalMatrix(Transform _transform)
	{
		glm::mat4 matT = glm::translate(glm::mat4(1.0f), _transform.position);
		glm::mat4 matR = glm::mat4_cast(glm::quat(glm::radians(_transform.rotation))); // 
		glm::mat4 matS = glm::scale(glm::mat4(1.0f), _transform.scale);

		Matrix4x4 out;
		out.glmMat = matT * matR * matS;

		return out;
	}

	Matrix4x4 Transform::GetLocalMatrix() const
	{
		Matrix4x4 localMatrix = CreateLocalMatrix(*this);
		return localMatrix;
	}

	bool Transform::Decompose(const Matrix4x4& _matrix, Vector3& _outTranslation, Vector3& _outRotation, Vector3& _outScale)
	{
		Quaternion rotation;
		Vector3 skew;
		Vector4 perspective;
		bool isSuccess = glm::decompose(_matrix.glmMat, _outScale, rotation, _outTranslation, skew, perspective);

	//	// 1. Translation 추출
	//// Row-Major 기준: 4행( _41, _42, _43 )이 위치입니다.
	//// Column-Major라면: 4열( [3][0], [3][1], [3][2] )을 가져오세요.
	//	_outTranslation.x = _matrix.mat[3][0];
	//	_outTranslation.y = _matrix.mat[3][1];
	//	_outTranslation.z = _matrix.mat[3][2];

	//	// 2. Scale 추출
	//	// 행렬의 각 축(Basis Vector)의 길이를 구합니다.
	//	// Row-Major 기준: 1행, 2행, 3행이 각각 X, Y, Z 축 벡터입니다.
	//	Vector3 axisX = Vector3(_matrix.mat[0][0], _matrix.mat[0][1], _matrix.mat[0][2]);
	//	Vector3 axisY = Vector3(_matrix.mat[1][0], _matrix.mat[1][1], _matrix.mat[1][2]);
	//	Vector3 axisZ = Vector3(_matrix.mat[2][0], _matrix.mat[2][1], _matrix.mat[2][2]);

	//	_outScale.x = axisX.length();
	//	_outScale.y = axisY.length();
	//	_outScale.z = axisZ.length();

	//	// [예외 처리] 스케일이 너무 작으면 회전을 구할 수 없음
	//	if (_outScale.x < 0.0001f || _outScale.y < 0.0001f || _outScale.z < 0.0001f)
	//	{
	//		rotation = Quaternion();
	//		return false;
	//	}

	//	// 3. Rotation 추출
	//	// 스케일이 적용된 축들을 정규화(Normalize)하여 순수 회전 행렬을 만듭니다.
	//	axisX /= _outScale.x;
	//	axisY /= _outScale.y;
	//	axisZ /= _outScale.z;

	//	// [중요] 음수 스케일(Negative Scale) 대응 (행렬식 판별)
	//	// 외적을 이용해 좌표계가 뒤집혔는지 확인합니다.
	//	Vector3 expectedZ = glm::cross(axisX, axisY);
	//	if (glm::dot(expectedZ, axisZ) < 0.0f)
	//	{
	//		// 뒤집혔다면 한 축의 스케일을 반전시킵니다.
	//		_outScale.x *= -1.0f;
	//		axisX *= -1.0f;
	//	}

	//	// 4. 회전 행렬을 쿼터니언으로 변환
	//	// 정규화된 축으로 임시 회전 행렬을 구성했다고 가정하고 쿼터니언 변환
	//	// (이 부분은 사용 중인 Math 라이브러리의 Rotation Matrix -> Quaternion 변환 함수를 쓰면 됩니다)
	//	Matrix4x4 rotationMat = Matrix4x4();
	//	rotationMat.mat[0][0] = axisX.x; rotationMat.mat[0][1] = axisX.y; rotationMat.mat[0][2] = axisX.z;
	//	rotationMat.mat[1][0] = axisY.x; rotationMat.mat[1][1] = axisY.y; rotationMat.mat[1][2] = axisY.z;
	//	rotationMat.mat[2][0] = axisZ.x; rotationMat.mat[2][1] = axisZ.y; rotationMat.mat[2][2] = axisZ.z;

	//	_outRotation = glm::degrees(glm::eulerAngles(glm::quat_cast(rotationMat.glmMat));

		if (!isSuccess)
		{
			return false;
		}

		_outRotation = glm::degrees(glm::eulerAngles(rotation));
		return true;
	}

	Transform Transform::Decompose(const Matrix4x4& _matrix)
	{
		Transform transform;
		if (Decompose(_matrix, transform.position, transform.rotation, transform.scale))
		{
			return transform;
		}
		else
		{
			return Transform();
		}
	}
}
