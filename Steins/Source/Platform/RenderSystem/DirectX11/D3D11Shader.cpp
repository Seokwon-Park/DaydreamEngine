#include "SteinsPCH.h"
#include "D3D11Shader.h"

namespace Steins
{
	D3D11Shader::D3D11Shader(const std::string& _vertexSrc, const std::string& _pixelSrc)
	{
		/*ID3DBlob* vsBlob;
		HRESULT hr = D3DCompile(
			_vertexSrc.c_str(),
			_vertexSrc.length(),
			nullptr,
			nullptr,
			nullptr,
			"vs_main", "vs_5_0",
			0, 0,
			&vsBlob,
			nullptr);

		hr = m_Context->GetD3DDevice()->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			&m_VertexShader);

		m_Context->GetD3DDevice()->CreateInputLayout(
			m_Context->GetInputElements().data(), UINT(m_Context->GetInputElements().size()),
			vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
			&m_InputLayout);
		ID3DBlob* psBlob;
		hr = D3DCompile(
			pixelSrc.c_str(),
			pixelSrc.length(),
			nullptr,
			nullptr,
			nullptr,
			"ps_main", "ps_5_0",
			0, 0,
			&psBlob,
			nullptr);

		hr = m_Context->GetD3DDevice()->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&m_PixelShader);

		vsBlob->Release();
		psBlob->Release();*/
	}
	void D3D11Shader::Bind() const
	{
	}

	void D3D11Shader::Unbind() const
	{
	}

	void D3D11Shader::SetMat4(const std::string& _name, const Matrix4x4& _value)
	{
	}
}
