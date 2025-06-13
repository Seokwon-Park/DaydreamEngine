#pragma once

namespace Steins
{
	enum class ProjectionType {Perspective, Orthographic};

	class Camera
	{
	public:
		Camera();
		~Camera();

		const Matrix4x4& GetViewMatrix() const;
		const Matrix4x4& GetProjectionMatrix() const;
		const Matrix4x4& GetViewProjectionMatrix() const;

		void SetPosition(Vector3 _position);

	private:
		void UpdateMatrix();

		Matrix4x4 projectionMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 viewProjectionMatrix;

		Vector3 position, rotation;
		Vector3 direction, up;
		float nearPlane, farPlane;
		ProjectionType projectionType;
	};
}