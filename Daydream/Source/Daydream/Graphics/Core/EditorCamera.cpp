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

		UpdateViewMatrix();

		prevMousePos = mouse;
	}

	void EditorCamera::CameraRotate()
	{
		transform.rotation.y += mouseDelta.x * rotationSpeed;
		transform.rotation.x += mouseDelta.y * rotationSpeed;

	}

	void EditorCamera::CameraMove(Float32 _deltaTime)
	{
		if (Input::GetKeyPressed(Key::Q))
		{
			SetPosition(transform.position - transform.GetUp() * _deltaTime);
		}
		if (Input::GetKeyPressed(Key::E))
		{
			SetPosition(transform.position + transform.GetUp() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::W))
		{
			SetPosition(transform.position + transform.GetForward() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::A))
		{
			SetPosition(transform.position - transform.GetRight() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::S))
		{
			SetPosition(transform.position - transform.GetForward() * _deltaTime);
		}

		if (Input::GetKeyPressed(Key::D))
		{
			SetPosition(transform.position + transform.GetRight() * _deltaTime);
		}
	}
	void EditorCamera::ControlCameraView(Float32 _deltaTime)
	{
		CameraRotate();
		CameraMove(_deltaTime);
	}
}
