#include "SteinsPCH.h"
#include "D3D11Shader.h"

namespace Steins
{
	D3D11PixelShader::D3D11PixelShader(const FilePath& _filepath)
		:D3D11Shader(_filepath, ShaderType::Pixel)
	{
	}
	D3D11PixelShader::D3D11PixelShader(const std::string& _src)
		:D3D11Shader(_src, ShaderType::Pixel)
	{
	}
	void D3D11PixelShader::Bind() const
	{
	}
	void D3D11PixelShader::Unbind() const
	{
	}
	void D3D11PixelShader::SetMat4(const std::string& _name, const Matrix4x4& _value)
	{
	}
}