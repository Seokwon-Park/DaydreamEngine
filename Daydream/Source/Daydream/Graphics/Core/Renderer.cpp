#include "DaydreamPCH.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Renderer2D.h"

#include <Daydream/Scene/Components/ModelRendererComponent.h>
#include "Daydream/Graphics/Utility/ModelLoader.h"
#include "Daydream/Graphics/Utility/ShaderCompileHelper.h"

#include "Daydream/Graphics/Resources/Mesh.h"

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
		renderContext = renderDevice->CreateContext(Renderer::maxFramesInFlight);
		commandLists.resize(maxCommandListsInFlight);
		for (auto& commandList : commandLists)
		{
			commandList = MakeUnique<RenderCommandList>();
		}
		ShaderCompileHelper::Init();
		/*	RenderCommand::Init(renderDevice.get());*/
	}

	void Renderer::Shutdown()
	{
		//Renderer2D::Shutdown();
		ShaderCompileHelper::Shutdown();
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

	void Renderer::OnWindowResize(UInt32 _width, UInt32 _height)
	{
		//renderContext->SetViewport(0, 0, _width, _height);
		//currentWindow->GetSwapchain()->ResizeSwapchain(_width, _height);
	}

	void Renderer::BeginFrame(Swapchain* _swapchain)
	{
		_swapchain->BeginFrame();
	}

	void Renderer::EndFrame(Swapchain* _swapchain)
	{
		_swapchain->EndFrame();
		_swapchain->Present();
	}

	void Renderer::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
		renderContext->BeginRenderPass(_renderPass, _framebuffer);
	}
	void Renderer::EndRenderPass(Shared<RenderPass> _renderPass)
	{
		renderContext->EndRenderPass(_renderPass);
	}

	void Renderer::BindPipelineState(Shared<PipelineState> _pso)
	{
		renderContext->BindPipelineState(_pso);
		//commandLists[0]->AddCommand([=]() {renderContext->BindPipelineState(_pso); });
	}

	void Renderer::BindMesh(Shared<Mesh> _mesh)
	{
		renderContext->BindMesh(_mesh);
	}

	void Renderer::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		renderContext->SetTexture2D(_name, _texture);
		//commandLists[0]->AddCommand([=]() {renderContext->SetTexture2D(_name, _texture); });
	}
	void Renderer::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		renderContext->SetTextureCube(_name, _textureCube);
		//commandLists[0]->AddCommand([=]() {renderContext->SetTextureCube(_name, _textureCube); });
	}
	void Renderer::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		renderContext->SetConstantBuffer(_name, _buffer);
		//commandLists[0]->AddCommand([=]() {renderContext->SetConstantBuffer(_name, _buffer); });
	}
	void Renderer::BindMaterial(Shared<Material> _material)
	{
		renderContext->BindMaterial(_material);
		//commandLists[0]->AddCommand([=]() {renderContext->SetMaterial(_material); });
	}

	void Renderer::DrawIndexed(UInt32 _indexCount)
	{
		//_shader->Bind();
		/*_shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		_shader->SetMat4("u_Transform", _transform);*/

		//_vertexArray->Bind();
		renderContext->DrawIndexed(_indexCount);
		//commandLists[0]->AddCommand([=]() {renderContext->DrawIndexed(_indexCount); });
	}

	void Renderer::Submit()
	{
		commandLists[0]->Execute();
	}

	void Renderer::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
		renderContext->CopyTexture2D(_src, _dst);
	}

	void Renderer::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
		renderContext->CopyTextureToCubemapFace(_dstCubemap, _faceIndex, _srcTexture2D, _mipLevel);

	}

	void Renderer::GenerateMips(Shared<Texture> _texture)
	{
		renderContext->GenerateMips(_texture);
	}

}
