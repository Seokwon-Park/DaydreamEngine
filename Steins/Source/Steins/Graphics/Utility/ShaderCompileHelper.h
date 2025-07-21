#pragma once
#include "dxc/dxcapi.h"

namespace Steins
{
	enum class ShaderType;

	class ShaderCompileHelper
	{
	public:
		static void Init();
		static void Shutdown();
		static void GetDXIL(const FilePath& _filepath, ShaderType _type, ComPtr<IDxcBlob>& _shaderBlob);
		static void GetDX12Reflection(ComPtr<IDxcBlob>& _shaderBlob, ComPtr<ID3D12ShaderReflection>& _reflection);
		static void ConvertHLSLtoSPIRV(const FilePath& _filepath, ShaderType _type, Array<UInt32>& _output);
		static void ConvertSPIRVtoHLSL(const FilePath& _filepath, ShaderType _type, Array<UInt32>& _output);
		static void ConvertSPIRVtoGLSL(const Array<UInt32> _spirvData, ShaderType _type, String& _glslSource);
	private:
		static IDxcUtils* utils;
		static IDxcCompiler3* compiler;
	};
}