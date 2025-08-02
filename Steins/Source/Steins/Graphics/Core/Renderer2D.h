#pragma once
#include "OrthographicCamera.h"

namespace Steins
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& _camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const Vector3& _position, const Vector2& _size, const Vector4& _color);
		static void DrawQuad(const Matrix4x4& _transform, const Vector4& _color);
	private:

	};
}