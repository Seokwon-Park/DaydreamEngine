#include "SteinsPCH.h"
#include "VertexArray.h"
#include "GraphicsDevice.h"

#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLVertexArray.h"
#include "Platform/RenderSystem/DirectX11/D3D11VertexArray.h"
#include "Platform/RenderSystem/DirectX12/D3D12VertexArray.h"

namespace Steins {

	Shared<VertexArray> VertexArray::Create()
	{
		return Renderer::GetRenderDevice()->CreateVertexArray();
	}

}