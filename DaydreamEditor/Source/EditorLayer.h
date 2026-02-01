#pragma once

#include <Daydream.h>
#include "EditorPanels/SceneHierarchyPanel.h"
#include "EditorPanels/ViewportPanel.h"
#include "EditorPanels/PropertyPanel.h"
#include "EditorPanels/AssetBrowserPanel.h"
#include "EditorPanels/SkyboxPanel.h"


namespace Daydream
{
	struct EntityInfo
	{
		UInt32 dummy[2]{};
		UInt32 entityID;
		Int32 thickness;
	};

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& _e);

		void CreateProject();

		void CreateDockspace();
		void UpdateViewportSize();

		Pair<Int32, Int32> GetViewportMousePos();
	private:
		Vector2 mainWindowSize;
		Vector2 viewportSize;
		Vector2 viewportBounds[2];

		bool isViewportHovered = false;
		bool isViewportFocused = false;
		bool isGuizmoInteract = false;
		bool viewportShouldResize = true;

		Unique<SceneRenderer> sceneRenderer;

		Shared<VertexBuffer> cubeVBO;
		Shared<IndexBuffer> cubeIBO;

		Shared<Sampler> sampler;
		Shared<Texture2D> texture;

		Shared<TextureCube> textureCube;

		Shared<ConstantBuffer> viewProjMat;
		EntityInfo info;
		Shared<ConstantBuffer> entityBuffer;

		//Shared<MouseScrolledEvent>
		Shared<RenderPass> depthRenderPass;
		Shared<RenderPass> gBufferRenderPass;
		Shared<RenderPass> renderPass;
		Shared<RenderPass> maskRenderPass;

		Shared<Framebuffer> depthFramebuffer;
		Shared<Framebuffer> gBufferFramebuffer;
		Shared<Framebuffer> viewportFramebuffer;
		Shared<Framebuffer> maskFramebuffer;

		Shared<PipelineState> depthPSO;
		Shared<PipelineState> gBufferPSO;
		Shared<PipelineState> deferredLightingPSO;
		Shared<PipelineState> pso;
		Shared<PipelineState> pso3d;
		Shared<PipelineState> skyboxPipeline;
		Shared<PipelineState> equirectangleToCubePipeline;
		Shared<PipelineState> maskPSO;

		Shared<Material> deferredLightingMaterial;
		Shared<Material> material;
		Shared<Material> material3d;
		Shared<Material> maskMaterial;

		Shared<Mesh> cubeMesh;
		Shared<Model> model;

		Shared<Scene> activeScene;

		Shared<EditorCamera> editorCamera;

		Shared<Project> currentProject;
		Unique<AssetManager> eidtorAssetManager;

		UInt32 viewIndex = 0;

		//Panels
		Unique<ViewportPanel> viewportPanel;
		Unique<PropertyPanel> propertyPanel;
		Unique<SceneHierarchyPanel> sceneHierarchyPanel;
		Unique<AssetBrowserPanel> assetBrowserPanel;
		Unique<SkyboxPanel> skyboxPanel;

		UInt32 guizmoType = -1;
	};
}
