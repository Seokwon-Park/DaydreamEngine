#include "DaydreamPCH.h"
#include "EditorCamera.h"

#include "Daydream/Core/Input.h"

namespace Daydream
{
	EditorCamera::EditorCamera()
		:Camera()
	{
	}

	void EditorCamera::Update(Float32 _deltaTime)
	{
		const Vector2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		if (Input::GetMousePressed(Mouse::ButtonRight))
		{
			Vector2 delta = (mouse - prevMousePos);
			//DAYDREAM_CORE_INFO("{}, {}", delta.x, delta.y);

			CameraRotate(delta);
			CameraMove(_deltaTime);
		}

		prevMousePos = mouse;
	}
	Quaternion EditorCamera::GetOrientation()
	{
		return Quaternion(Vector3(pitch, yaw, 0.0f));
	}
	Vector3 EditorCamera::GetForward()
	{
		return glm::rotate(GetOrientation(), dir);
	}
	Vector3 EditorCamera::GetUp()
	{
		return glm::rotate(GetOrientation(), up);
	}
	Vector3 EditorCamera::GetRight()
	{
		return glm::cross(GetUp(), GetForward());
	}
	void EditorCamera::CameraRotate(Vector2 _delta)
	{
		yaw += _delta.x * rotationSpeed;
		pitch -= _delta.y * rotationSpeed;

		UpdateViewMatrix();
	}

	void EditorCamera::CameraMove(Float32 _deltaTime)
	{
		if (Input::GetKeyPressed(Key::Q))
		{
			SetPosition(position - GetUp() * _deltaTime);
		}
		if (Input::GetKeyPressed(Key::E))
		{
			SetPosition(position + GetUp() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::W))
		{
			SetPosition(position + GetForward() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::A))
		{
			SetPosition(position - GetRight() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::S))
		{
			SetPosition(position - GetForward() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::D))
		{
			SetPosition(position + GetRight() * _deltaTime);
		}
	}

	void EditorCamera::UpdateViewMatrix()
	{

		viewMatrix = Matrix4x4::LookTo(position, GetForward(), GetUp());
		Camera::UpdateMatrix();
	}
}
