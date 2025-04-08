#pragma once

#include "GraphicsDevice.h"

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Steins
{
	class Renderer
	{
	public:
		static void Init(GraphicsDevice* _device);
		
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Shared<Shader>& _shader, const Shared<VertexArray>& _vertexArray);

		inline static RendererAPIType GetAPI() { return RendererAPI::GetRendererAPI(); }
	private:
		struct SceneData
		{
			Matrix4x4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}


