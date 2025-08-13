#pragma once

#include <Daydream.h>
#include "EditorPanels/SceneHierarchyPanel.h"
#include "EditorPanels/ViewportPanel.h"
#include "EditorPanels/PropertyPanel.h"

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
		bool isViewportHovered = false;
		bool viewportShouldResize = true;

		Shared<VertexBuffer> squareVB;
		Shared<VertexBuffer> squareVB2;
		Shared<IndexBuffer> squareIB;
		Shared<IndexBuffer> squareIB2;
		Shared<Texture2D> texture;
		Shared<Texture2D> texture2;

		Shared<ConstantBuffer> viewProjMat;
		Shared<ConstantBuffer> lightBuffer;

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

		Shared<EditorCamera> editorCamera;

		LightData lightData;

		ViewportPanel viewportPanel;
		PropertyPanel propertyPanel;
		SceneHierarchyPanel sceneHierarchyPanel;
		
	};
}
