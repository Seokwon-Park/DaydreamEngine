#pragma once


namespace Daydream
{
	class Matrix4x4;

	class Transform
	{
	public:
		Transform();
		Transform(const Vector3& pos);
		Transform(const Vector3& pos, const Vector3& rot);
		Transform(const Vector3& pos, const Vector3& rot, const Vector3& scl);

		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetRight();
		Quaternion GetOrientation();

		Transform* parent = nullptr;

		Vector3 position = Vector3();
		Vector3 rotation = Vector3();
		Vector3 scale = Vector3(1.0f);

		Matrix4x4 GetWorldMatrix() const;
	};
}