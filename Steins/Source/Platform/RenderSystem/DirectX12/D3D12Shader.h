#pragma once

#include "Steins/Render/Shader.h"
#include "D3D12RenderDevice.h"

namespace Steins
{
	class D3D12Shader : public Shader
	{
	public:
		D3D12Shader(D3D12RenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);

		void Bind() const override;
		void Unbind() const override;
		void* GetNativeHandle() override { return shaderBlob.Get(); };

		D3D12_SHADER_BYTECODE GetShaderBytecode() const { return shaderByteCode; }
	private:
		D3D12RenderDevice* device;
		D3D12_SHADER_BYTECODE shaderByteCode;
		ComPtr<ID3DBlob> shaderBlob;
	};
}