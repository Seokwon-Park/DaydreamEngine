#pragma once

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "RenderCommand.h"
#include "Daydream/Graphics/Resources/Shader.h"
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
		static void SetCurrentWindow(DaydreamWindow* _window) { instance->currentWindow = _window; }
		static DaydreamWindow* GetCurrentWindow() { return instance->currentWindow; }
		static void OnWindowResize(UInt32 _width, UInt32 _height);
		
		static void BeginSwapchainRenderPass(DaydreamWindow* _window);
		static void EndSwapchainRenderPass(DaydreamWindow* _window);

		static void BeginScene(const Camera& _camera);
		static void EndScene();

		static void Submit(UInt32 _indexCount);

		static Renderer& Get() { return *instance; }
		static inline RendererAPIType GetAPI() { return instance->renderDevice->GetAPI(); }
		static inline RenderDevice* GetRenderDevice() { return instance->renderDevice.get(); }

	private:
		Renderer(RendererAPIType _API);

		static Renderer* instance;

		DaydreamWindow* currentWindow;

		Unique<RenderDevice> renderDevice;


	};
}


