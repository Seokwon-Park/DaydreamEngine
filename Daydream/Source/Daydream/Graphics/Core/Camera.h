#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"

namespace Daydream
{
	enum class ProjectionType { Orthographic, Perspective };

	struct CameraConstantBufferData
	{
		Matrix4x4 view;
		Matrix4x4 projection;
		Matrix4x4 viewProjection;
	};

	class Camera
	{
	public:
		Camera();
		~Camera();

		virtual void Update(Float32 _deltaTime);

		const Matrix4x4& GetViewMatrix() const;
		const Matrix4x4& GetProjectionMatrix() const;
		const Matrix4x4& GetViewProjectionMatrix();
		Shared<ConstantBuffer> GetViewProjectionConstantBuffer() const { return viewProjectionBuffer; }

		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetRight();

		void SetPosition(Vector3 _position);
		inline Vector3 GetPosition() const { return transform.position; }

		void UpdateAspectRatio(UInt32 _width, UInt32 _height);

		void SetProjectionType(ProjectionType _type) { projectionType = _type; }

	protected:
		void UpdateViewProjectionMatrix();
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();

		Shared<ConstantBuffer> viewProjectionBuffer;

		Matrix4x4 projectionMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 viewProjectionMatrix;

		Transform transform;
		Float32 nearPlane, farPlane;
		Float32 fovy;
		Float32 orthoSize;
		Float32 aspectRatio;

		ProjectionType projectionType = ProjectionType::Perspective;
	};
}