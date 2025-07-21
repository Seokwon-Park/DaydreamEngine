#include "SteinsPCH.h"
#include "ShaderCompileHelper.h"
#include "Steins/Enum/RendererEnums.h"

#include <spirv_cross/spirv_glsl.hpp>

#include <Steins/Graphics/Utility/GraphicsUtil.h>
#include <shaderc/shaderc.h>

namespace Steins
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
	void ShaderCompileHelper::GetDXIL(const FilePath& _filepath, ShaderType _type, ComPtr<IDxcBlob>& _shaderBlob)
	{
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HRESULT hr = utils->LoadFile(_filepath.ToWString().c_str(), nullptr, sourceBlob.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to load File! {0}", _filepath.ToString());

		// 2. DxcBuffer ����
		DxcBuffer sourceBuffer{};
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		// 3. ������ ���� ����
		std::vector<LPCWSTR> args;

		WideString target = GraphicsUtil::GetShaderTargetNameW(_type, L"6_0");
		WideString entryPoint = GraphicsUtil::GetShaderEntryPointNameW(_type);

		// �⺻ �ɼ�
		args.push_back(L"-T");
		args.push_back(target.c_str());
		args.push_back(L"-E");
		args.push_back(entryPoint.c_str());
		args.push_back(L"-O3");

		ComPtr<IDxcResult> result;
		hr = compiler->Compile(&sourceBuffer,
			args.data(),
			static_cast<UINT32>(args.size()),
			nullptr,  // ��Ŭ��� �ڵ鷯 ����
			IID_PPV_ARGS(&result));

		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");

		// ���� Ȯ��
		ComPtr<IDxcBlobUtf8> errors;
		result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
		if (errors && errors->GetStringLength() > 0)
		{
			String errorMsg = errors->GetStringPointer();

			STEINS_CORE_ERROR("{0}", errorMsg);

			HRESULT status;
			result->GetStatus(&status);
			if (FAILED(status)) return;
		}

		// SPIR-V ���
		hr = result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(_shaderBlob.GetAddressOf()), nullptr);
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
	}

	void ShaderCompileHelper::GetDX12Reflection(ComPtr<IDxcBlob>& _shaderBlob, ComPtr<ID3D12ShaderReflection>& _reflection)
	{
		DxcBuffer reflectionBuffer = {};
		reflectionBuffer.Ptr = _shaderBlob->GetBufferPointer();
		reflectionBuffer.Size = _shaderBlob->GetBufferSize();

		HRESULT hr = utils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(_reflection.GetAddressOf()));
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create reflection!");
	}

	void ShaderCompileHelper::ConvertHLSLtoSPIRV(const FilePath& _filepath, ShaderType _type, Array<UInt32>& _output)
	{
		ComPtr<IDxcBlobEncoding> sourceBlob;
		HRESULT hr = utils->LoadFile(_filepath.ToWString().c_str(), nullptr, sourceBlob.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to load File! {0}", _filepath.ToString());

		// 2. DxcBuffer ����
		DxcBuffer sourceBuffer;
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		sourceBuffer.Encoding = DXC_CP_ACP; // LoadFile�� �ڵ����� ���ڵ� ����

		// 3. ������ ���� ����
		std::vector<LPCWSTR> args;

		WideString target = GraphicsUtil::GetShaderTargetNameW(_type, L"6_0");
		WideString entryPoint = GraphicsUtil::GetShaderEntryPointNameW(_type);

		// �⺻ �ɼ�
		args.push_back(L"-spirv");
		args.push_back(L"-T");
		args.push_back(target.c_str());
		args.push_back(L"-E");
		args.push_back(entryPoint.c_str());
		args.push_back(L"-fspv-reflect");
		args.push_back(L"-fvk-use-dx-layout");
		args.push_back(L"-O3");

		ComPtr<IDxcResult> result;
		hr = compiler->Compile(&sourceBuffer,
			args.data(),
			static_cast<UINT32>(args.size()),
			nullptr,  // ��Ŭ��� �ڵ鷯 ����
			IID_PPV_ARGS(&result));

		if (FAILED(hr)) return;

		// ���� Ȯ��
		ComPtr<IDxcBlobUtf8> errors;
		result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
		if (errors && errors->GetStringLength() > 0)
		{
			String errorMsg = errors->GetStringPointer();

			STEINS_CORE_ERROR("{0}", errorMsg);

			HRESULT status;
			result->GetStatus(&status);
			if (FAILED(status)) return ;
		}

		// SPIR-V ���
		ComPtr<IDxcBlob> spirvBlob;
		result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&spirvBlob), nullptr);
		if (!spirvBlob) return;

		_output.resize((spirvBlob->GetBufferSize()+sizeof(UInt32)-1)/sizeof(UInt32));
		memcpy(_output.data(), spirvBlob->GetBufferPointer(), spirvBlob->GetBufferSize());
	}
	void ShaderCompileHelper::ConvertSPIRVtoHLSL(const FilePath& _filepath, ShaderType _type, Array<UInt32>& _output)
	{
	}
	void ShaderCompileHelper::ConvertSPIRVtoGLSL(const Array<UInt32> _spirvData, ShaderType _type, String& _glslSource)
	{
		spirv_cross::CompilerGLSL compilerGLSL(_spirvData);

		// GLSL ������ �ɼ� ����
		spirv_cross::CompilerGLSL::Options options;
		options.version = 450;  // GLSL 3.30 ����
		options.es = false;     // OpenGL ES�� �ƴ� ����ũ�� OpenGL
		options.vulkan_semantics = false;  // Vulkan �ø�ƽ ��� ����
		options.emit_push_constant_as_uniform_buffer = false;
		options.separate_shader_objects = true;  

		compilerGLSL.set_common_options(options);
		// GLSL �ڵ� ����
		_glslSource = compilerGLSL.compile();
	}
}
