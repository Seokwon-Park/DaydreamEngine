#include "SteinsPCH.h"
#include "Renderer.h"

namespace Steins
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	RendererAPIType Renderer::API = RendererAPIType::None;

	void Renderer::Init(GraphicsDevice* _device)
	{
		API = _device->GetAPI();
		RenderCommand::Init(_device);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Shared<Shader>& _shader, const Shared<VertexArray>& _vertexArray, const Matrix4x4 _transform)
	{
		_shader->Bind();
		_shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		_shader->SetMat4("u_Transform", _transform);

		_vertexArray->Bind();
		RenderCommand::DrawIndexed(_vertexArray->GetIndexBuffer()->GetCount());
	}




}
