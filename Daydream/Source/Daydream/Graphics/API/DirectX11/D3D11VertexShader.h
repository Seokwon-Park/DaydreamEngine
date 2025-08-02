#pragma once

#include "D3D11Shader.h"

namespace Daydream
{
	class D3D11VertexShader : public D3D11Shader
	{
	public:
		D3D11VertexShader(D3D11RenderDevice* _device, const std::string& _src, const ShaderLoadMode& _mode);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11VertexShader> vertexShader;
	};
}