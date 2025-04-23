#include "SteinsPCH.h"
#include "Shader.h"

#include "Steins/Core/Application.h"
#include "Steins/Render/Renderer.h"

#include "Platform/RenderSystem/OpenGL/OpenGLShader.h"
#include "Platform/RenderSystem/DirectX11/D3D11Shader.h"


namespace Steins
{
	Shared<Shader> Shader::Create(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		return Application::GetGraphicsDevice()->CreateShader(_src, _type, _mode);

	}
}
