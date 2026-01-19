#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Shader.h"

#include "Daydream/Core/Application.h"
#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/API/OpenGL/OpenGLShader.h"
#include "Daydream/Graphics/API/DirectX11/D3D11Shader.h"


namespace Daydream
{
	Shared<Shader> Shader::Create(const String& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		return Renderer::GetRenderDevice()->CreateShader(_src, _type, _mode);
	}
}
