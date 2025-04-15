#include "SteinsPCH.h"
#include "D3D11Shader.h"

namespace Steins
{
	D3D11VertexShader::D3D11VertexShader(const FilePath& _filepath)
		:D3D11Shader(_filepath, ShaderType::Vertex)
	{
	}
	D3D11VertexShader::D3D11VertexShader(const std::string& _src)
		:D3D11Shader(_src, ShaderType::Vertex)
	{
	}
	void D3D11VertexShader::Bind() const
	{
	}
	void D3D11VertexShader::Unbind() const
	{
	}
	void D3D11VertexShader::SetMat4(const std::string& _name, const Matrix4x4& _value)
	{
	}
}