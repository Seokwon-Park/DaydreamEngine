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
		mouseDelta = (mouse - prevMousePos);
		//DAYDREAM_CORE_INFO("{}, {}", delta.x, delta.y);

		orientation = Quaternion(Vector3(pitch, yaw, 0.0f));
		dir = glm::rotate(orientation, Vector3(0.0f, 0.0f, 1.0f));
		up = glm::rotate(orientation, Vector3(0.0f, 1.0f, 0.0f));

		UpdateViewMatrix();

		prevMousePos = mouse;
	}

	void EditorCamera::CameraRotate()
	{
		yaw += mouseDelta.x * rotationSpeed;
		pitch += mouseDelta.y * rotationSpeed;

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
	void EditorCamera::ControlCameraView(Float32 _deltaTime)
	{
		CameraRotate();
		CameraMove(_deltaTime);
	}
}
