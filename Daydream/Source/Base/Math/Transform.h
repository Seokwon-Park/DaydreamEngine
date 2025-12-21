#pragma once


namespace Daydream
{
	class Matrix4x4;

	class Transform
	{
	public:
		Transform();
		Transform(const Vector3& _position);
		Transform(const Vector3& _position, const Vector3& _rotation);
		Transform(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale);

		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetRight();
		Quaternion GetOrientation();

		Vector3 position = Vector3();
		Vector3 rotation = Vector3();
		Vector3 scale = Vector3(1.0f);

		Matrix4x4 GetLocalMatrix() const;
		static Matrix4x4 CreateLocalMatrix(Transform _transform);
		static bool Decompose(const Matrix4x4& _matrix,
			Vector3& _outTranslation,
			Vector3& _outRotation, // 오일러(Vector3)보다 쿼터니언이 안정적입니다.
			Vector3& _outScale);

		static Transform Decompose(const Matrix4x4& _matrix);
	};
}