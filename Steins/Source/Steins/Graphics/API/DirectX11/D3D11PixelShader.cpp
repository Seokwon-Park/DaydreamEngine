#include "SteinsPCH.h"
#include "D3D11Shader.h"

namespace Steins
{
	D3D11PixelShader::D3D11PixelShader(D3D11RenderDevice* _device, const std::string& _src, const ShaderLoadMode& _mode)
		:D3D11Shader(_device, _src, ShaderType::Pixel, _mode)
	{
		device->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
	}

	void D3D11PixelShader::Bind() const
	{
		device->GetContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
	}
	void D3D11PixelShader::Unbind() const
	{
		device->GetContext()->PSSetShader(nullptr, nullptr, 0);
	}

}