#pragma once

#include "RenderCommandList.h"
#include "RenderCommandQueue.h"
#include "RenderThread.h"

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "Daydream/Graphics/Core/RenderContext.h"
#include "Daydream/Graphics/Core/Camera.h"
#include "Daydream/Graphics/Resources/Skybox.h"

namespace Daydream
{
	class Scene;
	class Renderer
	{
	public:
		static constexpr UInt32 MaxFramesInFlight = 3;
		static constexpr UInt32 MaxCommandListsInFlight = 2;

		template<typename RenderFunction>
		static void EnqueueCommand(RenderFunction&& _command)
		{
			if (useRenderThread)
			{
				// 멀티스레드
				commandQueues[recordingQueueIndex]->AddRenderCommand(std::forward<RenderFunction>(_command));
			}
			else
			{
				// 싱글스레드(디버깅)용 모드
				_command();
			}
		}

		template<typename RenderFunction>
		static void EnqueueSingleTimeCommand(RenderFunction&& _command)
		{
			singleTimeCommandQueue.push(_command);
		}

		//static RenderCommandList* GetCurrentCommandQueue();

		//Do Not Call twice
		static void Init(RendererAPIType _API);
		static void Shutdown();

		//This Function is only for OpenGL
		static void TransferContextForRenderThread(DaydreamWindow* _window);

		static bool CreateSwapchainForWindow(DaydreamWindow& _window);
		static void SetCurrentWindow(DaydreamWindow* _window) { currentWindow = _window; }
		static DaydreamWindow* GetCurrentWindow() { return currentWindow; }
		static void OnWindowResize(DaydreamWindow* _window, UInt32 _width, UInt32 _height);

		static void SetRenderThreadEnabled(bool _enabled);
		static bool IsRenderThreadEnabled() { return useRenderThread; }

		static void BeginFrame(Swapchain* _swapchain);
		static void EndFrame(Swapchain* _swapchain);

		static void BeginRenderPass(const Shared<RenderPass>& _renderPass, const Shared<Framebuffer>& _framebuffer);
		static void EndRenderPass(const Shared<RenderPass> &_renderPass);

		static void BeginSwapchainRenderPass(Swapchain* _swapchain);
		static void EndSwapchainRenderPass(Swapchain* _swapchain);

		static void BindPipelineState(Shared<PipelineState> _pipelineState);

		static void SetTexture2D(const String& _name, Shared<Texture2D> _texture);
		static void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube);
		static void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer);

		template <typename DataType>
		static void UpdateConstantBuffer(Shared<ConstantBuffer> _buffer, const DataType& _data)
		{
			EnqueueCommand([_buffer, _data]()
				{
					_buffer->Update(&_data, sizeof(DataType));
				});
		}

		static void BindMesh(Shared<Mesh> _mesh);
		static void BindMaterial(Shared<Material> _material);

		static void DrawIndexed(UInt32 _indexCount);

		static void ResizeFramebuffer(const Shared<Framebuffer>& _framebuffer, UInt32 _width, UInt32 _height);

		static void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst);
		static void CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel = 0);
		virtual void CopyTextureToBuffer(Shared<Texture2D> _srcTexture, Shared<GPUBuffer> _dstBuffer, UInt32 _offsetX, UInt32 _offsetY, UInt32 _width, UInt32 _height) = 0;
		static void GenerateMips(Shared<Texture> _texture);

		static void FlushSingleTimeCommands();

		static void Submit();

		static ImGuiRenderer* GetImGuiRenderer() { return imguiRenderer.get(); }

		//static Renderer& Get() { return *instance; }
		static inline RendererAPIType GetAPI() { return renderDevice->GetAPI(); }
		static inline RenderDevice* GetRenderDevice() { return renderDevice.get(); }
		static inline RenderContext* GetRenderContext() { return renderContext.get(); }
		static inline Skybox* GetSkybox() { return skybox.get(); }
		static inline RenderCommandList* GetActiveCommandList() { return renderContext->GetActiveCommandList().get(); }
	private:
		Renderer() = default;

		inline static DaydreamWindow* currentWindow = nullptr;
		inline static Unique<RenderDevice> renderDevice = nullptr;
		inline static Unique<RenderContext> renderContext = nullptr;
		inline static Unique<ImGuiRenderer> imguiRenderer = nullptr;
		inline static Unique<Skybox> skybox = nullptr;

		/////////////////////////////////  RenderThread  ///////////////////////////////// 
		inline static bool useRenderThread = 1;

		inline static Queue<RenderCommand> singleTimeCommandQueue;
		inline static Array<Unique<RenderCommandQueue>> commandQueues;
		//이 큐가 현재 렌더 스레드에서 사용중인지
		inline static std::array<std::atomic<bool>, MaxCommandListsInFlight> commandQueueBusyFlags;
		// 렌더스레드와 충돌 방지
		inline static std::mutex commandQueueStateMutex;
		inline static std::condition_variable commandQueueStateCV;

		inline static Unique<RenderThread> renderThread = nullptr;
		inline static UInt32 recordingQueueIndex = 0;
		//////////////////////////////////////////////////////////////////////////////////

	};
}


