#pragma once

namespace Daydream
{
	enum class ProjectionType { Orthographic, Perspective };

	class Camera
	{
	public:
		Camera();
		~Camera();

		virtual void Update(Float32 _deltaTime);

		const Matrix4x4& GetViewMatrix() const;
		const Matrix4x4& GetProjectionMatrix() const;
		const Matrix4x4& GetViewProjectionMatrix();

		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetRight();
		Quaternion GetOrientation();

		void SetPosition(Vector3 _position);
		inline Vector3 GetPosition() const { return position; }

		void UpdateAspectRatio(UInt32 _width, UInt32 _height);

		void SetProjectionType(ProjectionType _type) { projectionType = _type; }

	protected:
		void UpdateMatrix();
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();

		Matrix4x4 projectionMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 viewProjectionMatrix;

		Vector3 position, rotation;
		Vector3 dir, up;
		Quaternion orientation;
		Float32 nearPlane, farPlane;
		Float32 fovy;
		Float32 orthoSize;
		Float32 aspectRatio;


		ProjectionType projectionType = ProjectionType::Perspective;
	};
}