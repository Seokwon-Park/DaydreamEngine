#pragma once

#include <Daydream.h>
#include "EditorPanels/SceneHierarchyPanel.h"
#include "EditorPanels/ViewportPanel.h"
#include "EditorPanels/PropertyPanel.h"
#include "EditorPanels/AssetBrowserPanel.h"

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
		Shared<VertexBuffer> cubeVBO;
		Shared<IndexBuffer> squareIB;
		Shared<IndexBuffer> cubeIBO;
		Shared<Texture2D> texture;
		Shared<Texture2D> texture2;
		Shared<TextureCube> textureCube;

		Shared<ConstantBuffer> viewProjMat;
		Shared<ConstantBuffer> lightBuffer;

		Shared<Shader> vs;
		Shared<Shader> ps;

		Shared<Shader> vs3d;
		Shared<Shader> ps3d;

		Shared<Shader> vscube;
		Shared<Shader> pscube;
		//Shared<MouseScrolledEvent>
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> viewportFramebuffer;
		Shared<PipelineState> pso;
		Shared<PipelineState> pso3d;
		Shared<PipelineState> cubemapPipeline;

		Shared<Material> material;
		Shared<Material> material3d;
		Shared<Material> materialcube;

		Shared<Mesh> mesh;
		Shared<Model> model;

		Shared<Scene> activeScene;

		Shared<EditorCamera> editorCamera;

		LightData lightData;

		Unique<ViewportPanel> viewportPanel;
		Unique<PropertyPanel> propertyPanel;
		Unique<SceneHierarchyPanel> sceneHierarchyPanel;
		Unique<AssetBrowserPanel> assetBrowserPanel;
		
	};
}
