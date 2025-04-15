#pragma once

#include "Steins/Render/Shader.h"

namespace Steins
{
	class D3D11Shader: public Shader
	{
	public:
		D3D11Shader(const FilePath& _filepath, const ShaderType& _type);
		D3D11Shader(const std::string& _src, const ShaderType& _type);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

		static Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type);
		static Shared<Shader> CreateShader(const FilePath& _filepath, const ShaderType& _type);


	protected:
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;
	private:
		ComPtr<ID3D11DomainShader> ds;
		ComPtr<ID3D11HullShader> hs;
		ComPtr<ID3D11GeometryShader> gs;
		ComPtr<ID3D11PixelShader> ps;

	};

	class D3D11VertexShader : public D3D11Shader
	{
	public:
		D3D11VertexShader(const FilePath& _filepath);
		D3D11VertexShader(const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11VertexShader> vertexShader;
	};

	class D3D11PixelShader: public D3D11Shader
	{
	public:
		D3D11PixelShader(const FilePath& _filepath);
		D3D11PixelShader(const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11VertexShader> vertexShader;
	};
}


