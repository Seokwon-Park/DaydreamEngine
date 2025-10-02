#pragma once

#include <Daydream.h>
#include "EditorPanels/SceneHierarchyPanel.h"
#include "EditorPanels/ViewportPanel.h"
#include "EditorPanels/PropertyPanel.h"
#include "EditorPanels/AssetBrowserPanel.h"
#include "EditorPanels/SkyboxPanel.h"

namespace Daydream
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual void OnAttach() override;
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
		Shared<VertexBuffer> cubeVBO;
		Shared<IndexBuffer> squareIB;
		Shared<IndexBuffer> cubeIBO;

		Shared<Sampler> sampler;
		Shared<Texture2D> texture;

		Shared<TextureCube> textureCube;

		Shared<ConstantBuffer> viewProjMat;

		//Shared<MouseScrolledEvent>
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> viewportFramebuffer;

		Shared<PipelineState> pso;
		Shared<PipelineState> pso3d;
		Shared<PipelineState> skyboxPipeline;
		Shared<PipelineState> equirectangleToCubePipeline;

		Shared<Material> material;
		Shared<Material> material3d;
		Shared<Material> materialcube;

		Shared<Mesh> mesh;
		Shared<Model> model;

		Shared<Scene> activeScene;

		Shared<EditorCamera> editorCamera;


		Unique<ViewportPanel> viewportPanel;
		Unique<PropertyPanel> propertyPanel;
		Unique<SceneHierarchyPanel> sceneHierarchyPanel;
		Unique<AssetBrowserPanel> assetBrowserPanel;
		Unique<SkyboxPanel> skyboxPanel;
		
	};
}
