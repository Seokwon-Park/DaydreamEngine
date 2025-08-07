#pragma once

#include "Camera.h"
#include "Daydream/Event/Event.h"

namespace Daydream
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera();

		virtual void Update(Float32 _deltaTime) override;
		void OnEvent(Event& e);
		Quaternion GetOrientation();

		Vector3 GetForward();
		Vector3 GetUp();
		Vector3 GetRight();
	private:
		virtual void UpdateViewMatrix() override;

		void CameraRotate(Vector2 _delta);
		void CameraMove(Float32 _deltaTime);

		Vector2 prevMousePos{ 0, 0 };
		float pitch = 0.0f;
		float yaw = 0.0f;
		float rotationSpeed = 0.01f;
		float moveSpeed = 0.01f;
	};
}