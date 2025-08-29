#include "DaydreamPCH.h"
#include "ShaderCompileHelper.h"
#include "Daydream/Enum/RendererEnums.h"

#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_hlsl.hpp>

#include <Daydream/Graphics/Utility/GraphicsUtility.h>

#include <Daydream/Graphics/Resources/Shader.h>

namespace Daydream
{
	IDxcUtils* ShaderCompileHelper::utils = nullptr;
	IDxcCompiler3* ShaderCompileHelper::compiler = nullptr;

	void ShaderCompileHelper::Init()
	{
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	}

	void ShaderCompileHelper::Shutdown()
	{
		utils->Release();
		utils = nullptr;
		compiler->Release();
		compiler = nullptr;
	}
	void ShaderCompileHelper::GetDXIL(const Path& _filepath, ShaderType _type, ComPtr<IDxcBlob>& _shaderBlob)
	{
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HRESULT hr = utils->LoadFile(_filepath.ToWString().c_str(), nullptr, sourceBlob.GetAddressOf());
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to load File! {0}", _filepath.ToString());

		// 2. DxcBuffer 설정
		DxcBuffer sourceBuffer{};
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		// 3. 컴파일 인자 구성
		std::vector<LPCWSTR> args;

		WideString target = GraphicsUtility::GetShaderTargetNameW(_type, L"6_0");
		WideString entryPoint = GraphicsUtility::GetShaderEntryPointNameW(_type);

		// 기본 옵션
		args.push_back(L"-T");
		args.push_back(target.c_str());
		args.push_back(L"-E");
		args.push_back(entryPoint.c_str());
		args.push_back(L"-O3");

		ComPtr<IDxcResult> result;
		hr = compiler->Compile(&sourceBuffer,
			args.data(),
			static_cast<UINT32>(args.size()),
			nullptr,  // 인클루드 핸들러 없음
			IID_PPV_ARGS(&result));

		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");

		// 에러 확인
		ComPtr<IDxcBlobUtf8> errors;
		result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
		if (errors && errors->GetStringLength() > 0)
		{
			String errorMsg = errors->GetStringPointer();

			DAYDREAM_CORE_ERROR("{0}", errorMsg);

			HRESULT status;
			result->GetStatus(&status);
			if (FAILED(status)) return;
		}

		// SPIR-V 결과
		hr = result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(_shaderBlob.GetAddressOf()), nullptr);
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
	}

	void ShaderCompileHelper::GetDX12Reflection(ComPtr<IDxcBlob>& _shaderBlob, ComPtr<ID3D12ShaderReflection>& _reflection)
	{
		DxcBuffer reflectionBuffer = {};
		reflectionBuffer.Ptr = _shaderBlob->GetBufferPointer();
		reflectionBuffer.Size = _shaderBlob->GetBufferSize();

		HRESULT hr = utils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(_reflection.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create reflection!");
	}

	void ShaderCompileHelper::ConvertHLSLtoSPIRV(const Path& _filepath, ShaderType _type, Array<UInt32>& _output)
	{
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HRESULT hr = utils->LoadFile(_filepath.ToWString().c_str(), nullptr, sourceBlob.GetAddressOf());
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to load File! {0}", _filepath.ToString());

		// 2. DxcBuffer 설정
		DxcBuffer sourceBuffer;
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		sourceBuffer.Encoding = DXC_CP_ACP; // LoadFile이 자동으로 인코딩 감지

		// 3. 컴파일 인자 구성
		std::vector<LPCWSTR> args;

		WideString target = GraphicsUtility::GetShaderTargetNameW(_type, L"6_0");
		WideString entryPoint = GraphicsUtility::GetShaderEntryPointNameW(_type);

		// 기본 옵션
		args.push_back(L"-T");
		args.push_back(target.c_str());
		args.push_back(L"-E");
		args.push_back(entryPoint.c_str());
		args.push_back(L"-spirv");
		args.push_back(L"-fspv-reflect");
		if (_type == ShaderType::Vertex)
		{
			args.push_back(L"-fvk-invert-y");
		}
		args.push_back(L"-fvk-use-dx-layout");
		args.push_back(L"-O3");

		ComPtr<IDxcResult> result;
		hr = compiler->Compile(&sourceBuffer,
			args.data(),
			static_cast<UINT32>(args.size()),
			nullptr,  // 인클루드 핸들러 없음
			IID_PPV_ARGS(&result));

		if (FAILED(hr)) return;

		// 에러 확인
		ComPtr<IDxcBlobUtf8> errors;
		result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
		if (errors && errors->GetStringLength() > 0)
		{
			String errorMsg = errors->GetStringPointer();

			DAYDREAM_CORE_ERROR("{0}", errorMsg);

			HRESULT status;
			result->GetStatus(&status);
			if (FAILED(status)) return;
		}

		// SPIR-V 결과
		ComPtr<IDxcBlob> spirvBlob;
		result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&spirvBlob), nullptr);
		if (!spirvBlob) return;

		_output.resize((spirvBlob->GetBufferSize() + sizeof(UInt32) - 1) / sizeof(UInt32));
		memcpy(_output.data(), spirvBlob->GetBufferPointer(), spirvBlob->GetBufferSize());
	}
	void ShaderCompileHelper::ConvertSPIRVtoDXBC(const Array<UInt32> _spirvData, ShaderType _type, String& _hlslSource)
	{
		spirv_cross::CompilerHLSL hlsl(_spirvData);

		spirv_cross::CompilerGLSL::Options commonOptions{};
		commonOptions.vertex.flip_vert_y = true;

		hlsl.set_common_options(commonOptions);
		
		spirv_cross::CompilerHLSL::Options options{};
		options.shader_model = 50;
		options.use_entry_point_name = true;
		
		hlsl.set_hlsl_options(options);

		_hlslSource = hlsl.compile();
	}
	void ShaderCompileHelper::ConvertSPIRVtoGLSL(const Array<UInt32> _spirvData, ShaderType _type, String& _glslSource)
	{
		spirv_cross::CompilerGLSL compilerGLSL(_spirvData);

		// GLSL 컴파일 옵션 설정
		spirv_cross::CompilerGLSL::Options options{};
		options.version = 450;  
		options.separate_shader_objects = true;
		//options.vertex.flip_vert_y = true;
				
		compilerGLSL.set_common_options(options);
		// GLSL 코드 생성
		_glslSource = compilerGLSL.compile();
	}
}
