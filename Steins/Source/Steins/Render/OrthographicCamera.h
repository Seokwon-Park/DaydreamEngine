#pragma once

namespace Steins
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float _left, float _right, float _bottom, float _top);

		inline const Vector3 GetPosition() { return position;  }
		inline void SetPosition(const Vector3& _position) { position = _position; CalculateViewMatrix(); }

		inline const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }
		inline const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
		inline const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

	private:
		void CalculateViewMatrix();

		Matrix4x4 projectionMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 viewProjectionMatrix;

		Vector3 position;
		float rotation = 0.0f;
	};
}

