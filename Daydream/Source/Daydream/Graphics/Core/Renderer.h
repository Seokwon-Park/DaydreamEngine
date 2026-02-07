#pragma once

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "RenderCommand.h"
#include "RenderCommandList.h"
#include "Daydream/Scene/Scene.h"
#include "Daydream/Graphics/Core/Camera.h"

namespace Daydream
{
	class Renderer
	{
	public:
		static constexpr UInt32 maxFramesInFlight = 3;
		static constexpr UInt32 maxCommandListsInFlight = 2;

		static void BeginCommandList() { renderContext->BeginCommandList(); }
		static void EndCommandList() { renderContext->EndCommandList(); }

		template<typename Func>
		static void Record(Func&& _command)
		{
			commandLists[0]->AddCommand(std::forward<Func>(_command));
		}

		//static RenderCommandList* GetCurrentCommandQueue();

		//Do Not Call twice
		static void Init(RendererAPIType _API);
		static void Shutdown();
		static void CreateSwapchainForWindow(DaydreamWindow* _window);
		static void SetCurrentWindow(DaydreamWindow* _window) { currentWindow = _window; }
		static DaydreamWindow* GetCurrentWindow() { return currentWindow; }
		static void OnWindowResize(UInt32 _width, UInt32 _height);


		static void BeginFrame(Swapchain* _swapchain);
		static void EndFrame(Swapchain* _swapchain);
		

		static void BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer); 
		static void EndRenderPass(Shared<RenderPass> _renderPass); 

		static void BindPipelineState(Shared<PipelineState> _pipelineState);

		static void BindMesh(Shared<Mesh> _mesh);

		static void SetTexture2D(const String& _name, Shared<Texture2D> _texture);
		static void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube);
		static void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer);
		static void BindMaterial(Shared<Material> _material);

		static void DrawIndexed(UInt32 _indexCount);

		static void Submit();

		static void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst);
		static void CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel = 0);
		static void GenerateMips(Shared<Texture> _texture);

		static ImGuiRenderer* GetImGuiRenderer() { return imguiRenderer.get(); }

		//static Renderer& Get() { return *instance; }
		static inline RendererAPIType GetAPI() { return renderDevice->GetAPI(); }
		static inline RenderDevice* GetRenderDevice() { return renderDevice.get(); }
		static inline RenderContext* GetRenderContext() { return renderContext.get(); }

	private:
		Renderer() = default;

		inline static DaydreamWindow* currentWindow = nullptr;

		inline static Unique<RenderDevice> renderDevice = nullptr;
		inline static Unique<RenderContext> renderContext = nullptr;
		inline static Unique<ImGuiRenderer> imguiRenderer = nullptr;

		inline static Array<Unique<RenderCommandList>> commandLists;
	};
}


