#include "SteinsPCH.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Steins
{
	std::unordered_map<std::string, SteinsWindow*> Renderer::windows;
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Unique<RenderDevice> Renderer::renderDevice = nullptr;

	void Renderer::Init(RendererAPIType _API)
	{
		renderDevice = RenderDevice::Create(_API);
		STEINS_CORE_ASSERT(renderDevice, "Failed to create graphics device!");
		renderDevice->Init();
		Renderer2D::Init();
		RenderCommand::Init(renderDevice.get());
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		renderDevice.reset();
	}

	void Renderer::RegisterWindow(std::string _name, SteinsWindow* _window)
	{
		renderDevice->CreateSwapChainForWnd(_window);
		windows.insert({ _name, _window });
	}

	void Renderer::OnWindowResize(UInt32 _width, UInt32 _height)
	{
		RenderCommand::SetViewport(_width, _height);
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
