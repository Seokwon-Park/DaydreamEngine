#include "SteinsPCH.h"
#include "Camera.h"

namespace Steins
{
	Camera::Camera()
	{
	}
	Camera::~Camera()
	{
	}
	const Matrix4x4& Camera::GetViewMatrix() const
	{
		return Matrix4x4();
	}
	const Matrix4x4& Camera::GetProjectionMatrix() const
	{
		return Matrix4x4();
	}
	const Matrix4x4& Camera::GetViewProjectionMatrix() const
	{
		return Matrix4x4();
	}
	void Camera::SetPosition(Vector3 _position)
	{
		position = _position;
		UpdateMatrix();
	}
	void Camera::UpdateMatrix()
	{
		viewMatrix = Matrix4x4::LookAtLH(position, direction, up);
		switch (projectionType)
		{
		case Steins::ProjectionType::Perspective:

			break;
		case Steins::ProjectionType::Orthographic:
			break;
		default:
			break;
		}
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
}