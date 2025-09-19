#pragma once
#include "dxc/dxcapi.h"

namespace Daydream
{
	enum class ShaderType;

	class ShaderCompileHelper
	{
	public:
		static void Init();
		static void Shutdown();
		static void GetDXIL(const Path& _filepath, ShaderType _type, ComPtr<IDxcBlob>& _shaderBlob);
		static void GetDX12Reflection(ComPtr<IDxcBlob>& _shaderBlob, ComPtr<ID3D12ShaderReflection>& _reflection);
		static void ConvertHLSLtoSPIRV(const Path& _filepath, ShaderType _type, Array<UInt32>& _output);
		static String ConvertSPIRVtoDXBC(const Array<UInt32> _spirvData, ShaderType _type);
		static String ConvertSPIRVtoGLSL(const Array<UInt32> _spirvData, ShaderType _type);
	private:
		inline static ShaderCompileHelper* instance;

		ComPtr<IDxcUtils> utils;
		ComPtr<IDxcCompiler3> compiler;
	};
}