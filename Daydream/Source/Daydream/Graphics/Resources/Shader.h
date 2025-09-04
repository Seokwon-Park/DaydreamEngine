#pragma once

#include "Daydream/Enum/RendererEnums.h"

namespace Daydream
{
	enum class ShaderResourceType
	{
		Input,
		ConstantBuffer,
		Texture,
		Sampler,
	};
	struct ShaderReflectionData
	{
		UInt32 set; // or location
		UInt32 binding;
		UInt32 descriptorTableIndex; // DX12;
		UInt32 count;
		UInt64 size;
		ShaderType shaderType;
		ShaderResourceType shaderResourceType;
		String name;
		RenderFormat format; // only for input layouts
	};

	struct ShaderProgram
	{

	};
	
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() = 0;

		ShaderType GetType() const { return shaderType; }
		Array<ShaderReflectionData>& GetShaderReflectionData() { return reflectionDatas; }

		static Shared<Shader> Create(const String& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
	protected:
		ShaderType shaderType;

		Array<ShaderReflectionData> reflectionDatas;
	};

}