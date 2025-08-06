#include "DaydreamPCH.h"
//#include "EditorCamera.h"
//
//#include "Daydream/Core/Input.h"
//
//namespace Daydream
//{
//	EditorCamera::EditorCamera()
//		:Camera()
//	{
//	}
//
//	void EditorCamera::Update(Float32 _deltaTime)
//	{
//		if (Input::GetMousePressed(Mouse::ButtonRight))
//		{
//			const Vector2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
//			Vector2 delta = (mouse - prevMousePos);
//			DAYDREAM_CORE_INFO("{}, {}", delta.x, delta.y);
//			prevMousePos = mouse;
//
//			MouseRotate(delta);
//
//			if (Input::GetKeyPressed(Key::D))
//			{
//				SetPosition(GetPosition() + Vector3(1.0f, 0.0f, 0.0f));
//			}
//		}
//	}
//	Quaternion EditorCamera::GetOrientation()
//	{
//		return Quaternion(Vector3(-pitch, -yaw, 1.0f));
//	}
//	void EditorCamera::MouseRotate(Vector2 _delta)
//	{
//		yaw += _delta.x * rotationSpeed;
//		pitch += _delta.y * rotationSpeed;
//
//
//
//		UpdateViewMatrix();
//	}
//
//	void EditorCamera::UpdateViewMatrix()
//	{
//		Vector3 newDir = glm::rotate(GetOrientation(), dir);
//		Vector3 newUp = glm::rotate(GetOrientation(), up);
//		viewMatrix = Matrix4x4::LookTo(position, newDir, newUp);
//		Camera::UpdateMatrix();
//	}
//}
