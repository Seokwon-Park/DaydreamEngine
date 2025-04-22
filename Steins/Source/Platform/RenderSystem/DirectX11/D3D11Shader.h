#pragma once

#include "Steins/Render/Shader.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(GraphicsDevice* _device, const FilePath& _filepath, const ShaderType& _type);
		D3D11Shader(GraphicsDevice* _device, const std::string& _src, const ShaderType& _type);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		std::string chooseTarget();
		std::string chooseEntryPoint();

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

		static Shared<Shader> CreateShader(GraphicsDevice* _device, const std::string& _src, const ShaderType& _type);
		static Shared<Shader> CreateShader(GraphicsDevice* _device, const FilePath& _filepath, const ShaderType& _type);
	protected:
		D3D11GraphicsDevice* device;
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;
	private:
	};

	class D3D11VertexShader : public D3D11Shader
	{
	public:
		D3D11VertexShader(GraphicsDevice* _device, const FilePath& _filepath);
		D3D11VertexShader(GraphicsDevice* _device, const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11InputLayout> inputLayout;
	};

	class D3D11PixelShader : public D3D11Shader
	{
	public:
		D3D11PixelShader(GraphicsDevice* _device, const FilePath& _filepath);
		D3D11PixelShader(GraphicsDevice* _device, const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11PixelShader> pixelShader;
	};

	class D3D11HullShader : public D3D11Shader
	{
	public:
		D3D11HullShader(const FilePath& _filepath);
		D3D11HullShader(const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11HullShader> hullShader;
	};

	class D3D11DomainShader : public D3D11Shader
	{
	public:
		D3D11DomainShader(const FilePath& _filepath);
		D3D11DomainShader(const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11DomainShader> domainShader;
	};

	class D3D11GeometryShader : public D3D11Shader
	{
	public:
		D3D11GeometryShader(const FilePath& _filepath);
		D3D11GeometryShader(const std::string& _src);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;

	private:
		ComPtr<ID3D11GeometryShader> geometryShader;
	};
}


