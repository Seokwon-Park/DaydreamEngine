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

		Shared<Shader> vs;
		Shared<Shader> ps;
		//Steins::Shared<Steins::MouseScrolledEvent>
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> viewportFramebuffer;
		Shared<PipelineState> pso;


		OrthographicCamera camera = Steins::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
		Matrix4x4 cameraPos;
	};
}
