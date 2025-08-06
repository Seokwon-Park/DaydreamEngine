#pragma once

//#include "Camera.h"
//#include "Daydream/Event/Event.h"
//
//namespace Daydream
//{
//	class EditorCamera : public Camera
//	{
//	public:
//		EditorCamera();
//
//		void Update(Float32 _deltaTime);
//		void OnEvent(Event& e);
//		Quaternion GetOrientation();
//	private:
//		virtual void UpdateViewMatrix() override;
//		virtual void UpdateProjectionMatrix() { Camera::GetProjectionMatrix(); };
//
//		void MouseRotate(Vector2 _delta);
//
//		Vector2 prevMousePos{ 0, 0 };
//		float pitch = 0.0f;
//		float yaw = 0.0f;
//		float rotationSpeed = 0.1f;
//	};
//}