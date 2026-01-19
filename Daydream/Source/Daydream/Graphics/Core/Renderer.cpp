#include "DaydreamPCH.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Renderer2D.h"

#include <Daydream/Scene/Components/ModelRendererComponent.h>
#include "Daydream/Graphics/Utility/ModelLoader.h"
#include "Daydream/Graphics/Utility/ShaderCompileHelper.h"

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
		renderer = renderDevice->CreateImGuiRenderer();
		renderContext = renderDevice->CreateContext();
		ShaderCompileHelper::Init();
		/*	RenderCommand::Init(renderDevice.get());*/
	}

	void Renderer::Shutdown()
	{
		//Renderer2D::Shutdown();
		ShaderCompileHelper::Shutdown();
		renderer->Shutdown();
		renderDevice.reset();
		//delete instance;
		//instance = nullptr;
	}

	void Renderer::CreateSwapchainForWindow(DaydreamWindow* _window)
	{
		renderDevice->CreateSwapchainForWindow(_window);
	}

	void Renderer::OnWindowResize(UInt32 _width, UInt32 _height)
	{
		renderContext->SetViewport(0, 0, _width, _height);
		currentWindow->GetSwapchain()->ResizeSwapchain(_width, _height);
	}

	void Renderer::BeginSwapchainRenderPass(DaydreamWindow* _window)
	{
		_window->GetSwapchain()->BeginRenderPass();
	}

	void Renderer::EndSwapchainRenderPass(DaydreamWindow* _window)
	{
		_window->GetSwapchain()->EndRenderPass();

	}


	void Renderer::EndScene()
	{

	}


	void Renderer::Submit(UInt32 _indexCount)
	{
		//_shader->Bind();
		/*_shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		_shader->SetMat4("u_Transform", _transform);*/

		//_vertexArray->Bind();
		renderContext->DrawIndexed(_indexCount);
	}


}
