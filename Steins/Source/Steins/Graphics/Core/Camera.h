#pragma once

namespace Steins
{
	enum class ProjectionType { Orthographic, Perspective };

	class Camera
	{
	public:
		Camera();
		~Camera();

		const Matrix4x4& GetViewMatrix() const;
		const Matrix4x4& GetProjectionMatrix() const;
		const Matrix4x4& GetViewProjectionMatrix() const;

		void SetPosition(Vector3 _position);
		inline Vector3 GetPosition() const { return position; }

		void SetProjectionType(ProjectionType _type) { projectionType = _type; }

	private:
		void UpdateMatrix();

		Matrix4x4 projectionMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 viewProjectionMatrix;

		Vector3 position, rotation;
		Vector3 direction, up;
		Float32 nearPlane, farPlane;
		Float32 fovy;
		Float32 orthoSize;
		ProjectionType projectionType = ProjectionType::Perspective;
	};
}