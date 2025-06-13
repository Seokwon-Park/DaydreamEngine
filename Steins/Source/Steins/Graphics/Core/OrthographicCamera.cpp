#include "SteinsPCH.h"
#include "OrthographicCamera.h"


namespace Steins
{
	OrthographicCamera::OrthographicCamera(float _left, float _right, float _bottom, float _top)
		: projectionMatrix(Matrix4x4::Orthographic(_left, _right, _bottom, _top)), viewMatrix()
	{
		viewProjectionMatrix = projectionMatrix * viewMatrix;

	}
	void OrthographicCamera::SetProjection(float _left, float _right, float _bottom, float _top)
	{
		projectionMatrix = Matrix4x4::Orthographic(_left, _right, _bottom, _top);
	}
	void OrthographicCamera::CalculateViewMatrix()
	{
		Matrix4x4 transform = Matrix4x4::Translate(Matrix4x4(), position);
			//glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		viewMatrix = Matrix4x4::Inverse(transform);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
}
