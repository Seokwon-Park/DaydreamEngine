#include "DaydreamPCH.h"
#include "Camera.h"

namespace Daydream
{
	Camera::Camera()
		:viewMatrix(Matrix4x4()), projectionMatrix(Matrix4x4())
	{
		dir= Vector3(0.0f, 0.0f, 1.0f);
		up = Vector3(0.0f, 1.0f, 0.0f);
		fovy = 70.0f;
		aspectRatio = 1.6f/ 0.9f;
		orthoSize = 5.0f;
		nearPlane = 0.1f;
		farPlane = 1000.0f;
		UpdateProjectionMatrix();
	}
	Camera::~Camera()
	{
	}

	void Camera::Update(Float32 _deltaTime)
	{

	}

	const Matrix4x4& Camera::GetViewMatrix() const
	{
		return viewMatrix;
	}
	const Matrix4x4& Camera::GetProjectionMatrix() const
	{
		return projectionMatrix;
	}
	const Matrix4x4& Camera::GetViewProjectionMatrix()
	{
		return viewProjectionMatrix;
	}

	Quaternion Camera::GetOrientation()
	{
		return orientation;
	}

	Vector3 Camera::GetForward()
	{
		return dir;
	}
	Vector3 Camera::GetUp()
	{
		return glm::rotate(GetOrientation(), Vector3(0.0f, 1.0f, 0.0f));
	}
	Vector3 Camera::GetRight()
	{
		return glm::cross(GetUp(), GetForward());
	}

	void Camera::SetPosition(Vector3 _position)
	{
		position = _position;
		UpdateViewMatrix();
	}
	void Camera::UpdateAspectRatio(UInt32 _width, UInt32 _height)
	{
		aspectRatio = static_cast<Float32>(_width) / _height;
		UpdateProjectionMatrix();
	}
	void Camera::UpdateMatrix()
	{
		viewProjectionMatrix = projectionMatrix * viewMatrix;
		viewProjectionMatrix.MatrixTranspose();
	}
	void Camera::UpdateViewMatrix()
	{
		viewMatrix = Matrix4x4::LookTo(position, dir, up);

		UpdateMatrix();
	}
	void Camera::UpdateProjectionMatrix()
	{
		switch (projectionType)
		{
		case ProjectionType::Perspective:
			projectionMatrix = Matrix4x4::Perspective(glm::radians(fovy), aspectRatio, nearPlane, farPlane);
			break;
		case ProjectionType::Orthographic:
		{
			projectionMatrix = Matrix4x4::Orthographic(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, nearPlane, farPlane);
			break;
		}
		default:
			break;
		}
		UpdateMatrix();
	}
}