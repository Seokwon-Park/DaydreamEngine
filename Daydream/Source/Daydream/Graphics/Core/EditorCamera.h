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

		void CameraRotate();
		void CameraMove(Float32 _deltaTime);

		void ControlCameraView(Float32 _deltaTime);
	private:
		Vector2 mouseDelta = Vector2(0, 0);
		Vector2 prevMousePos = Vector2(0, 0);

		float rotationSpeed = 1.0f;
		float moveSpeed = 0.01f;
		bool isDragging = false;
	};
}