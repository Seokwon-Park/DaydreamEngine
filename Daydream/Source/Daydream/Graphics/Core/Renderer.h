#pragma once

#include "RenderCommandList.h"
#include "RenderCommandQueue.h"
#include "RenderThread.h"

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "Daydream/Graphics/Core/RenderContext.h"
#include "Daydream/Graphics/Camera/Camera.h"
#include "Daydream/Graphics/Resources/Skybox.h"
#include "Daydream/Graphics/Resources/Texture/TextureView.h"

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
		static void EnqueuePreFrameCommand(RenderFunction&& _command)
		{
			singleTimeCommandQueue.push(_command);
		}

		static void Init(RendererAPIType _API);
		static void PostInit();
		static void Shutdown();

		static bool CreateSwapchainForWindow(DaydreamWindow& _window);
		static void OnSwapchainResize(Swapchain* _swapchain, UInt32 _width, UInt32 _height);

		static void SetRenderThreadEnabled(bool _enabled);
		static bool IsRenderThreadEnabled() { return useRenderThread; }

		static void BeginFrame(Swapchain* _swapchain);
		static void EndFrame(Swapchain* _swapchain);

		static void BeginRendering(const RenderingInfo& _renderingInfo);
		static void EndRendering(const RenderingInfo& _renderingInfo);
		static void BeginRendering(Swapchain* _swapchain);
		//static void BeginRenderPass(const Shared<RenderPass>& _renderPass, const Shared<Framebuffer>& _framebuffer);
		//static void EndRenderPass(const Shared<RenderPass>& _renderPass);

		//static void BeginSwapchainRenderPass(Swapchain* _swapchain);
		//static void EndSwapchainRenderPass(Swapchain* _swapchain);

		static void BindPipelineState(Shared<PipelineState> _pipelineState);

		//static void SetTexture2D(const String& _name, Shared<Texture2D> _texture);
		//static void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube);
		static void BindShaderResourceView(const String& _name, Shared<TextureView> _textureView, Shared<Sampler> _samplerState);
		static void BindConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer);
		

		template <typename DataType>
		static void UpdateConstantBuffer(Shared<ConstantBuffer> _buffer, const DataType& _data)
		{
			EnqueueCommand([_buffer, _data]()
				{
					_buffer->UpdateData(&_data, sizeof(DataType));
				});
		}

		static void BindMesh(Shared<Mesh> _mesh);
		static void BindMaterial(Shared<Material> _material);

		static void DrawIndexed(UInt32 _indexCount);

		static void RequestResizeFramebuffer(const Shared<Framebuffer>& _framebuffer, UInt32 _width, UInt32 _height);

		static void CopyBuffer(Shared<GPUBuffer> _src, Shared<GPUBuffer> _dst, UInt32 _copySize);
		static void CopyBufferToTexture(Shared<GPUBuffer> _src, Shared<GPUTexture> _dst, UInt32 _width, UInt32 _height);

		static void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst);
		static void CopyTexture2DToTextureCube(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel = 0);
		static void CopyTextureCubeToTexture2D(Shared<TextureCube> _srcCubemap, UInt32 _faceIndex, Shared<Texture2D> _dstTexture2D, UInt32 _mipLevel = 0);

		static void TransitionTextureState(Shared<GPUTexture> _texture,
			ResourceState _beforeState,
			ResourceState _afterState,
			UInt32 _mipLevel = 0,       // Texture 전용
			UInt32 _mipCount = 1);

		static void TransitionBufferState(
			Shared<GPUBuffer> _buffer,
			ResourceState _beforeState,
			ResourceState _afterState
		);

		static void GenerateMips(Shared<Texture> _texture);

		static void ExecutePreFrameCommands();

		static void Submit();

		inline static ImGuiRenderer* GetImGuiRenderer() { return imguiRenderer.get(); }

		//static Renderer& Get() { return *instance; }
		inline static RendererAPIType GetAPI() { return renderDevice->GetAPI(); }
		inline static RenderDevice* GetRenderDevice() { return renderDevice.get(); }
		inline static RenderContext* GetRenderContext() { return renderContext.get(); }
		inline static Skybox* GetSkybox() { return skybox.get(); }
		inline static RenderCommandList* GetActiveCommandList() { return renderContext->GetActiveCommandList().get(); }
	private:
		Renderer() = default;

		inline static Unique<RenderDevice> renderDevice = nullptr;
		inline static Unique<RenderContext> renderContext = nullptr;
		inline static Unique<ImGuiRenderer> imguiRenderer = nullptr;
		inline static Unique<Skybox> skybox = nullptr;

		/////////////////////////////////  RenderThread  ///////////////////////////////// 
		inline static bool useRenderThread = 0;

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


