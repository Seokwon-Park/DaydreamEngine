#include "SteinsPCH.h"
#include "Steins/Graphics/Resources/Shader.h"

#include "Steins/Core/Application.h"
#include "Steins/Graphics/Core/Renderer.h"

#include "Steins/Graphics/API/OpenGL/OpenGLShader.h"
#include "Steins/Graphics/API/DirectX11/D3D11Shader.h"


namespace Steins
{
	Shared<Shader> Shader::Create(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		return Renderer::GetRenderDevice()->CreateShader(_src, _type, _mode);

	}
}
