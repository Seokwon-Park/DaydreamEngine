#pragma once

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "RenderCommand.h"
#include "Daydream/Scene/Scene.h"
#include "Daydream/Graphics/Core/Camera.h"

namespace Daydream
{
	class Renderer
	{
	public:
		//Do Not Call twice
		static void Init(RendererAPIType _API);
		static void Shutdown();
		static void CreateSwapchainForWindow(DaydreamWindow* _window);
		static void SetCurrentWindow(DaydreamWindow* _window) { currentWindow = _window; }
		static DaydreamWindow* GetCurrentWindow() { return currentWindow; }
		static void OnWindowResize(UInt32 _width, UInt32 _height);

		static void BeginSwapchainRenderPass(DaydreamWindow* _window);
		static void EndSwapchainRenderPass(DaydreamWindow* _window);

		static void BeginScene(const Camera& _camera);
		static void EndScene();

		static void Submit(UInt32 _indexCount);

		static ImGuiRenderer* GetImGuiRenderer() { return renderer.get(); }

		//static Renderer& Get() { return *instance; }
		static inline RendererAPIType GetAPI() { return renderDevice->GetAPI(); }
		static inline RenderDevice* GetRenderDevice() { return renderDevice.get(); }

	private:
		Renderer() = default;

		inline static DaydreamWindow* currentWindow = nullptr;

		inline static Unique<RenderDevice> renderDevice = nullptr;
		inline static Unique<RenderContext> renderContext = nullptr;
		inline static Unique<ImGuiRenderer> renderer = nullptr;
	};
}


