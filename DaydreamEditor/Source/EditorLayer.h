#pragma once

#include <Daydream.h>

namespace Daydream
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual void OnUpdate(Float32 _deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnDetach() override;
		void CreateDockspace();
		void UpdateViewportSize(); 
	private:
		Vector2 mainWindowSize;
		Vector2 viewportSize;
		bool viewportSizeInit= false;

		Daydream::Shared<Daydream::VertexBuffer> squareVB;
		Daydream::Shared<Daydream::VertexBuffer> squareVB2;
		Daydream::Shared<Daydream::IndexBuffer> squareIB;
		Daydream::Shared<Daydream::IndexBuffer> squareIB2;
		Daydream::Shared<Daydream::ConstantBuffer> viewProjMat;
		Daydream::Shared<Daydream::Texture2D> texture;
		Daydream::Shared<Daydream::Texture2D> texture2;

		Shared<Shader> vs;
		Shared<Shader> ps;
		//Daydream::Shared<Daydream::MouseScrolledEvent>
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> viewportFramebuffer;
		Shared<PipelineState> pso;

		Daydream::Shared<Daydream::Material> material;

		Camera camera = Camera();
		Matrix4x4 cameraPos;
	};
}
