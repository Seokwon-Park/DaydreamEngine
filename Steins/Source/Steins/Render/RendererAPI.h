#pragma once

namespace Steins
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None,
			OpenGL,
			DirectX11,
		};
		virtual void SetClearColor(const Color& _color) = 0;
		virtual void Clear() = 0;

		//virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		static inline API GetAPI() { return API::OpenGL; }

	};
}