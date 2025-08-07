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
		bool isViewportFocused = false;
		bool viewportShouldResize = true;

		Shared<VertexBuffer> squareVB;
		Shared<VertexBuffer> squareVB2;
		Shared<IndexBuffer> squareIB;
		Shared<IndexBuffer> squareIB2;
		Shared<ConstantBuffer> viewProjMat;
		Shared<Texture2D> texture;
		Shared<Texture2D> texture2;

		Shared<Shader> vs;
		Shared<Shader> ps;

		Shared<Shader> vs3d;
		Shared<Shader> ps3d;
		//Shared<MouseScrolledEvent>
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> viewportFramebuffer;
		Shared<PipelineState> pso;
		Shared<PipelineState> pso3d;

		Shared<Material> material;
		Shared<Material> material3d;

		Shared<Model> model;

		Shared<Scene> activeScene;

		Shared<Camera> camera;
		Matrix4x4 cameraPos;
	};
}
