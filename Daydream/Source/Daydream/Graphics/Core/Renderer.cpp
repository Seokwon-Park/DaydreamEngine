#include "DaydreamPCH.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Renderer2D.h"

#include <Daydream/Scene/Components/ModelRendererComponent.h>
#include "Daydream/Scene/Scene.h"
#include "Daydream/Graphics/Utility/ModelLoader.h"
#include "Daydream/Graphics/Utility/ShaderCompileHelper.h"

#include "Daydream/Graphics/Resources/Mesh.h"
#include "Daydream/Graphics/Resources/Skybox.h"


namespace Daydream
{
	//Renderer* Renderer::instance = nullptr;

	void Renderer::Init(RendererAPIType _API)
	{
		//DAYDREAM_CORE_ASSERT(instance == nullptr, "Renderer Already Initialized!");
		//instance = new Renderer(_API);
		currentWindow = nullptr;
		renderDevice = RenderDevice::Create(_API);
		DAYDREAM_CORE_ASSERT(renderDevice, "Failed to create graphics device!");
		renderDevice->Init();
		imguiRenderer = renderDevice->CreateImGuiRenderer();
		renderContext = renderDevice->CreateContext();
		skybox = MakeUnique<Skybox>();
		commandQueues.resize(maxCommandListsInFlight);
		for (auto& commandList : commandQueues)
		{
			commandList = MakeUnique<RenderCommandQueue>();
		}

		// 모든 큐의 상태를 '사용 중이 아님(false)'으로 초기화합니다.
		// memory_order_relaxed는 동기화 순서가 중요하지 않은 단순 초기화에 사용
		for (auto& queueBusy : commandQueueBusyFlags)
		{
			queueBusy.store(false, std::memory_order_relaxed);
		}

		recordingQueueIndex = 0;

		renderThread = MakeUnique<RenderThread>();
		if (useRenderThread)
		{
			renderThread->Start();
		}

		ShaderCompileHelper::Init();
		/*	RenderCommand::Init(renderDevice.get());*/
	}

	void Renderer::Shutdown()
	{
		//Renderer2D::Shutdown();
		ShaderCompileHelper::Shutdown();
		if (useRenderThread)
		{
			renderThread->Stop();
			renderThread = nullptr;
		}
		skybox = nullptr;
		imguiRenderer->Shutdown();
		renderContext.reset();
		renderDevice.reset();
		//delete instance;
		//instance = nullptr;
	}

	void Renderer::CreateSwapchainForWindow(DaydreamWindow* _window)
	{
		SwapchainDesc desc;
		desc.width = _window->GetWidth();
		desc.height = _window->GetHeight();
		desc.bufferCount = Renderer::maxFramesInFlight;
		desc.format = RenderFormat::R8G8B8A8_UNORM;
		desc.isFullscreen = false;
		desc.isVSync = _window->IsVSync();

		Shared<Swapchain> swapchain = renderDevice->CreateSwapchain(_window, desc);
		_window->SetSwapchain(swapchain);
	}

	void Renderer::OnWindowResize(DaydreamWindow* _window, UInt32 _width, UInt32 _height)
	{
		//renderContext->SetViewport(0, 0, _width, _height);
		_window->GetSwapchain()->ResizeSwapchain(_width, _height);
	}

	void Renderer::SetRenderThreadEnabled(bool _enabled)
	{
		useRenderThread = _enabled;
		if (!renderThread)
		{
			return;
		}

		if (useRenderThread)
		{
			renderThread->Start();
		}
		else
		{
			renderThread->Stop();
		}
	}

	void Renderer::BeginFrame(Swapchain* _swapchain)
	{
		Enqueue([_swapchain]()
			{
				renderContext->SetActiveCommandList(_swapchain->GetCurrentCommandList());
				_swapchain->BeginFrame();
			});
	}

	void Renderer::EndFrame(Swapchain* _swapchain)
	{
		Enqueue([_swapchain]()
			{
				_swapchain->EndFrame();
				_swapchain->Present();
			});
	}

	void Renderer::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
		Enqueue([_renderPass, _framebuffer]()
			{
				renderContext->BeginRenderPass(_renderPass, _framebuffer);
			});
	}
	void Renderer::EndRenderPass(Shared<RenderPass> _renderPass)
	{
		Enqueue([_renderPass]()
			{
				renderContext->EndRenderPass(_renderPass);
			});
	}

	void Renderer::BeginSwapchainRenderPass(Swapchain* _swapchain)
	{
		Enqueue([_swapchain]()
			{
				renderContext->BeginRenderPass(_swapchain->GetRenderPass(), _swapchain->GetCurrentFramebuffer());
			});
	}

	void Renderer::EndSwapchainRenderPass(Swapchain* _swapchain)
	{
		Enqueue([_swapchain]()
			{
				renderContext->EndRenderPass(_swapchain->GetRenderPass());
			});
	}

	void Renderer::BindPipelineState(Shared<PipelineState> _pipelineState)
	{
		Enqueue([_pipelineState]()
			{
				renderContext->BindPipelineState(_pipelineState);
			});
	}

	void Renderer::BindMesh(Shared<Mesh> _mesh)
	{
		Enqueue([_mesh]()
			{
				renderContext->BindMesh(_mesh);
			});
	}

	void Renderer::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		Enqueue([_name, _texture]()
			{
				renderContext->SetTexture2D(_name, _texture);
			});
	}
	void Renderer::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		Enqueue([_name, _textureCube]()
			{
				renderContext->SetTextureCube(_name, _textureCube);
			});
	}
	void Renderer::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		Enqueue([_name, _buffer]()
			{
				renderContext->SetConstantBuffer(_name, _buffer);
			});
	}
	void Renderer::BindMaterial(Shared<Material> _material)
	{
		Enqueue([_material]()
			{
				renderContext->BindMaterial(_material);
			});
	}

	void Renderer::DrawIndexed(UInt32 _indexCount)
	{
		Enqueue([_indexCount]()
			{
				renderContext->DrawIndexed(_indexCount);
			});
	}

	void Renderer::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
		Enqueue([_src, _dst]()
			{
				renderContext->CopyTexture2D(_src, _dst);
			});
	}

	void Renderer::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
		Enqueue([_dstCubemap, _faceIndex, _srcTexture2D, _mipLevel]()
			{
				renderContext->CopyTextureToCubemapFace(_dstCubemap, _faceIndex, _srcTexture2D, _mipLevel);
			});

	}

	void Renderer::GenerateMips(Shared<Texture> _texture)
	{
		Enqueue([_texture]()
			{
				renderContext->GenerateMips(_texture);
			});
	}

	void Renderer::FlushSingleTimeCommands()
	{
		//Queue<FunctionPtr<void()>> queuedCommands;
		//std::swap(queuedCommands, singleTimeCommandQueue);
		while (!singleTimeCommandQueue.empty())
		{
			singleTimeCommandQueue.front()();
			singleTimeCommandQueue.pop();
		}
	}

	void Renderer::Submit()
	{
		// 현재 메인스레드가 작성한 큐의 인덱스와 포인터 가져오기
		const UInt32 submittedQueueIndex = recordingQueueIndex;
		RenderCommandQueue* submittedQueue = commandQueues[submittedQueueIndex].get();

		if (useRenderThread && renderThread)
		{
			// 큐의 상태를 사용중으로 잠금
			// memory_order_release는 이 시점 이전의 모든 메모리 기록이 다른 스레드에게 확실히 보이도록 보장
			commandQueueBusyFlags[submittedQueueIndex].store(true, std::memory_order_release);

			// 렌더스레드에 비동기 실행 요청 + 작업이 끝나면 실행할 함수
			renderThread->Submit(submittedQueue, [submittedQueueIndex]()
				{
					// [렌더 스레드에서 실행됨] 작업이 끝났으므로 큐 상태를 '사용 가능(false)'으로 변경
					commandQueueBusyFlags[submittedQueueIndex].store(false, std::memory_order_release);
					// wait하고 있을 commandQueueStateCV에게 작업이 끝났음을 알림
					commandQueueStateCV.notify_all();
				});

			// 새로 명령을 추가할 큐의 인덱스로 업데이트
			recordingQueueIndex = (recordingQueueIndex + 1) % maxCommandListsInFlight;
			std::unique_lock<std::mutex> lock(commandQueueStateMutex);
			commandQueueStateCV.wait(lock, []()
				{
					// 메인 스레드가 다음으로 쓰려고 하는 큐를, 렌더 스레드가 아직도 지우지 못하고있다면
					// 메인 스레드는 CPU를 점유하지 않고 여기서 대기
					// 렌더 스레드가 아까 제출할 콜백을 통해 false로 바꿔주면 깨어나서 다음 프레임 로직을 진행
					return !commandQueueBusyFlags[recordingQueueIndex].load(std::memory_order_acquire);
				});
		}
		else
		{
			//싱글 스레드일 경우 그냥 실행
			submittedQueue->Execute();
		}
	}

}
