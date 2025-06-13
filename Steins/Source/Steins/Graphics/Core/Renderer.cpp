#include "SteinsPCH.h"
#include "Steins/Graphics/Core/Renderer.h"
#include "Renderer2D.h"

namespace Steins
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(RendererAPIType _API)
	{
		currentWindow = nullptr;
		renderDevice = RenderDevice::Create(_API);
		STEINS_CORE_ASSERT(renderDevice, "Failed to create graphics device!");
	}

	void Renderer::Init(RendererAPIType _API)
	{
		//두 번 호출하지 말 것.
		STEINS_CORE_ASSERT(instance == nullptr, "Renderer Already Initialized!");
		instance = new Renderer(_API);
		Get().renderDevice->Init();
		RenderCommand::Init(Get().renderDevice.get());
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		Get().renderDevice.reset();
		delete instance;
		instance = nullptr;
	}

	void Renderer::RegisterWindow(std::string _name, SteinsWindow* _window)
	{
		Get().renderDevice->CreateSwapChainForWnd(_window);
		Get().windows.insert({ _name, _window });
	}

	void Renderer::OnWindowResize(UInt32 _width, UInt32 _height)
	{
		RenderCommand::SetViewport(_width, _height);
		Get().currentWindow->GetSwapChain()->ResizeSwapChain(_width, _height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(UInt32 _indexCount, const Matrix4x4 _transform)
	{
		//_shader->Bind();
		/*_shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		_shader->SetMat4("u_Transform", _transform);*/

		//_vertexArray->Bind();
		RenderCommand::DrawIndexed(_indexCount);
	}


}
