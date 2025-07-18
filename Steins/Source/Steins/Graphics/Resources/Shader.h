#pragma once

#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	enum class ShaderResourceType
	{
		ConstantBuffer,
		Texture2D,
		Sampler,
	};
	struct ShaderResourceDesc
	{
		String name;
		UInt32 set;
		UInt32 binding;
		ShaderResourceType type;
		UInt32 count;
		UInt32 size;
		ShaderType shaderType;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() = 0;

		ShaderType GetType() const { return shaderType; }
		Array<ShaderResourceDesc>& GetResourceInfo() { return resourceInfo; }

		static Shared<Shader> Create(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
	protected:
		ShaderType shaderType;

		Array<ShaderResourceDesc> resourceInfo;
	};

}