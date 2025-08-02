#pragma once

#include "Daydream/Graphics/Core/RenderDevice.h"

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Daydream/Graphics/Resources/Shader.h"

namespace Daydream
{
	class Renderer
	{
	public:
		//Do not Call twice
		static void Init(RendererAPIType _API);
		static void Shutdown();
		static void RegisterWindow(String _name, DaydreamWindow* _window);
		static void CreateSwapchainFor(DaydreamWindow* _window);
		static void SetCurrentWindow(DaydreamWindow* _window) { instance->currentWindow = _window; }
		static void SetCurrentWindow(String _name) { SetCurrentWindow(instance->windows[_name]); }
		static void OnWindowResize(UInt32 _width, UInt32 _height);
		
		static void BeginSwapchainFramebuffer();
		static void EndSwapchainFramebuffer();

		static void BeginScene(const Camera& _camera);
		static void EndScene();

		static void Submit(UInt32 _indexCount, const Matrix4x4 _transform = Matrix4x4());

		static Renderer& Get() { return *instance; }
		static inline RendererAPIType GetAPI() { return Get().renderDevice->GetAPI(); }
		static inline RenderDevice* GetRenderDevice() { return Get().renderDevice.get(); }

	private:
		Renderer(RendererAPIType _API);

		static Renderer* instance;

		DaydreamWindow* currentWindow;
		std::unordered_map<std::string, DaydreamWindow*> windows;

		Unique<RenderDevice> renderDevice;

		struct SceneData
		{
			Matrix4x4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}


