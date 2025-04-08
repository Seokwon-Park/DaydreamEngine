#pragma once

#include "Steins/Render/Shader.h"

namespace Steins
{
	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(const std::string& _vertexSrc, const std::string& _pixelSrc);

		void Bind() const override;
		void Unbind() const override;
		void SetMat4(const std::string& _name, const Matrix4x4& _value) override;
	private:
		ComPtr<ID3D11VertexShader> vs;
		ComPtr<ID3D11PixelShader> ps;
		
	};
}


