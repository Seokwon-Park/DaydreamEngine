#include "DaydreamPCH.h"
#include "Transform.h"

#include "Matrix4x4.h"

namespace Daydream
{
	Transform::Transform()
	{
	}
	Transform::Transform(const Vector3& pos)
	{
	}
	Transform::Transform(const Vector3& pos, const Vector3& rot)
	{
	}
	Transform::Transform(const Vector3& pos, const Vector3& rot, const Vector3& scl)
	{
	}

	Vector3 Transform::GetForward()
	{
		return glm::rotate(Quaternion(rotation), Vector3(0.0f,0.0f,1.0f));
	}
	Vector3 Transform::GetUp()
	{
		return glm::rotate(Quaternion(rotation), Vector3(0.0f, 1.0f, 0.0f));
	}
	Vector3 Transform::GetRight()
	{
		return glm::cross(GetUp(), GetForward());
	}

	Matrix4x4 Transform::GetWorldMatrix() const
	{
		Matrix4x4 localMatrix = Matrix4x4::TransformToWorldMatrix(*this);

		// 2. 부모가 있다면, 부모의 월드 행렬을 먼저 계산하고 곱해줍니다.
		if (parent)
		{
			return parent->GetWorldMatrix() * localMatrix;
		}

		localMatrix.MatrixTranspose();
		// 3. 부모가 없다면, 자신의 로컬 행렬이 곧 월드 행렬입니다.
		return localMatrix;
	}
}
