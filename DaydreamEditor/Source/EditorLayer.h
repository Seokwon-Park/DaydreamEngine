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
		virtual void OnEvent(Event& _event) override;

		bool OnKeyPressed(KeyPressedEvent& _e);

		void CreateProject();

		void CreateDockspace();
		void UpdateViewportSize();
	private:
		Vector2 mainWindowSize;
		Vector2 viewportSize;

		bool isViewportHovered = false;
		bool isViewportFocused = false;
		bool viewportShouldResize = true;

		Shared<VertexBuffer> cubeVBO;
		Shared<IndexBuffer> cubeIBO;

		Shared<Sampler> sampler;
		Shared<Texture2D> texture;

		Shared<TextureCube> textureCube;

		Shared<ConstantBuffer> viewProjMat;

		//Shared<MouseScrolledEvent>
		Shared<RenderPass> gBufferRenderPass;
		Shared<RenderPass> renderPass;
		Shared<Framebuffer> gBufferFramebuffer;
		Shared<Framebuffer> viewportFramebuffer;

		Shared<PipelineState> gBufferPSO;
		Shared<PipelineState> deferredLightingPSO;
		Shared<PipelineState> PSO;
		Shared<PipelineState> pso;
		Shared<PipelineState> pso3d;
		Shared<PipelineState> skyboxPipeline;
		Shared<PipelineState> equirectangleToCubePipeline;

		Shared<Material> deferredLightingMaterial;
		Shared<Material> material;
		Shared<Material> material3d;
		Shared<Material> materialcube;

		Shared<Mesh> mesh;
		Shared<Model> model;

		Shared<Scene> activeScene;

		Shared<EditorCamera> editorCamera;

		Shared<Project> currentProject;
		Unique<AssetManager> eidtorAssetManager;

		//Panels
		Unique<ViewportPanel> viewportPanel;
		Unique<PropertyPanel> propertyPanel;
		Unique<SceneHierarchyPanel> sceneHierarchyPanel;
		Unique<AssetBrowserPanel> assetBrowserPanel;
		Unique<SkyboxPanel> skyboxPanel;

		UInt32 guizmoType = -1;
	};
}
