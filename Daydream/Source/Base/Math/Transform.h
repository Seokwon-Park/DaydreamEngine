#pragma once

namespace Daydream
{
	class Transform
	{
	public:
		Transform();
		Transform(const Vector3& _position);
		Transform(const Vector3& _position, const Vector3& _rotation);
		Transform(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale);

		//Vector3 GetForward();
		//Vector3 GetUp();
		//Vector3 GetRight();
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;
		Quaternion GetOrientation();

		Vector3 position = Vector3();
		Vector3 rotation = Vector3();
		Vector3 scale = Vector3(1.0f);

		Matrix4x4 GetLocalMatrix() const;
		static Matrix4x4 CreateLocalMatrix(Transform _transform);
		static bool Decompose(const Matrix4x4& _worldMat,
			Vector3& _outPosition,
			Vector3& _outRotation,
			Vector3& _outScale);

		static Transform Decompose(const Matrix4x4& _matrix);
	};
}