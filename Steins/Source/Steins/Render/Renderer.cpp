#include "SteinsPCH.h"
#include "Renderer.h"

namespace Steins
{
	std::unordered_map<std::string, SteinsWindow*> Renderer::windows;
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Unique<GraphicsDevice> Renderer::renderDevice = nullptr;

	void Renderer::Init(RendererAPIType _API)
	{
		renderDevice = GraphicsDevice::Create(_API);
		STEINS_CORE_ASSERT(renderDevice, "Failed to create graphics device!");
		renderDevice->Init();
		RenderCommand::Init(renderDevice.get());
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

	void Renderer::BeginScene(OrthographicCamera& camera)
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
