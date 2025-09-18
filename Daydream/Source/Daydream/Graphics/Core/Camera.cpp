#include "DaydreamPCH.h"
#include "Camera.h"

namespace Daydream
{
	Camera::Camera()
		:viewMatrix(Matrix4x4()), projectionMatrix(Matrix4x4())
	{
		fovy = 60.0f;
		aspectRatio = 1.6f/ 0.9f;
		orthoSize = 5.0f;
		nearPlane = 0.001f;
		farPlane = 1000.0f;
		
		viewProjectionBuffer = ConstantBuffer::Create(sizeof(Matrix4x4));

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


	Vector3 Camera::GetForward()
	{
		return transform.GetForward();
	}
	Vector3 Camera::GetUp()
	{
		return transform.GetUp();
	}
	Vector3 Camera::GetRight()
	{
		return glm::cross(GetUp(), GetForward());
	}

	void Camera::SetPosition(Vector3 _position)
	{
		transform.position = _position;
		UpdateViewMatrix();
	}
	void Camera::UpdateAspectRatio(UInt32 _width, UInt32 _height)
	{
		aspectRatio = static_cast<Float32>(_width) / _height;
		UpdateProjectionMatrix();
	}
	void Camera::UpdateViewProjectionMatrix()
	{
		viewProjectionMatrix = projectionMatrix * viewMatrix;
		viewProjectionMatrix.MatrixTranspose();

		viewProjectionBuffer->Update(&viewProjectionMatrix, sizeof(Matrix4x4));
	}
	void Camera::UpdateViewMatrix()
	{
		viewMatrix = Matrix4x4::LookTo(transform.position, transform.GetForward(), transform.GetUp());
		UpdateViewProjectionMatrix();
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
		UpdateViewProjectionMatrix();
	}
}