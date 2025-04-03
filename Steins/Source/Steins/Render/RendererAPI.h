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

		static API GetAPI() { return graphicsAPI; }
		std::string GetAPIName() const;
	protected:
		inline static API graphicsAPI = API::None;
	};
}