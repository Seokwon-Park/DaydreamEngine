#pragma once

#include <Steins.h>

namespace Steins
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual void OnUpdate(Float32 _deltaTime) override;
		virtual void OnImGuiRender() override;
		void CreateDockspace();
	private:

		Vector2 viewportSize;

		Steins::Shared<Steins::VertexBuffer> squareVB;
		Steins::Shared<Steins::VertexBuffer> squareVB2;
		Steins::Shared<Steins::IndexBuffer> squareIB;
		Steins::Shared<Steins::IndexBuffer> squareIB2;
		Steins::Shared<Steins::ConstantBuffer> viewProjMat;
		Steins::Shared<Steins::Texture2D> texture;
		Steins::Shared<Steins::Texture2D> texture2;

		Shared<Shader> vs;
		Shared<Shader> ps;
		//Steins::Shared<Steins::MouseScrolledEvent>
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> viewportFramebuffer;
		Shared<PipelineState> pso;

		Steins::Shared<Steins::Material> material;

		OrthographicCamera camera = Steins::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
		Matrix4x4 cameraPos;
	};
}
