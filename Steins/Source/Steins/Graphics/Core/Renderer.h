#pragma once

#include "Steins/Graphics/Core/RenderDevice.h"

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Steins/Graphics/Resources/Shader.h"

namespace Steins
{
	class Renderer
	{
	public:
		//Do not Call twice
		static void Init(RendererAPIType _API);
		static void Shutdown();
		static void RegisterWindow(String _name, SteinsWindow* _window);
		static void CreateSwapchainFor(SteinsWindow* _window);
		static void SetCurrentWindow(SteinsWindow* _window) { instance->currentWindow = _window; }
		static void SetCurrentWindow(String _name) { SetCurrentWindow(instance->windows[_name]); }
		static void OnWindowResize(UInt32 _width, UInt32 _height);
		
		static void BeginSwapchainFramebuffer();
		static void EndSwapchainFramebuffer();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(UInt32 _indexCount, const Matrix4x4 _transform = Matrix4x4());

		static Renderer& Get() { return *instance; }
		static inline RendererAPIType GetAPI() { return Get().renderDevice->GetAPI(); }
		static inline RenderDevice* GetRenderDevice() { return Get().renderDevice.get(); }

	private:
		Renderer(RendererAPIType _API);

		static Renderer* instance;

		SteinsWindow* currentWindow;
		std::unordered_map<std::string, SteinsWindow*> windows;

		Unique<RenderDevice> renderDevice;

		struct SceneData
		{
			Matrix4x4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}


