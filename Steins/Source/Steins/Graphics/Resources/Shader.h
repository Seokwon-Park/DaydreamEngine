#pragma once

#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	enum class ShaderResourceType
	{
		Input,
		ConstantBuffer,
		Texture2D,
		Sampler,
	};
	struct ShaderReflectionInfo
	{
		UInt32 set;
		UInt32 binding;
		UInt32 count;
		UInt32 size;
		ShaderType shaderType;
		ShaderResourceType shaderResourceType;
		String name;
		RenderFormat format; // only for input layouts
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() = 0;

		ShaderType GetType() const { return shaderType; }
		Array<ShaderReflectionInfo>& GetReflectionInfo() { return reflectionInfo; }

		static Shared<Shader> Create(const String& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
	protected:
		ShaderType shaderType;

		Array<ShaderReflectionInfo> reflectionInfo;
	};

}