#pragma once

#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	enum class ShaderResourceType
	{
		ConstantBuffer,
		Texture,
	};
	struct ShaderResource
	{
		ShaderResourceType type;
		UInt32 set;
		UInt32 binding;
		UInt32 size;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() = 0;

		ShaderType GetType() const { return type; }

		static Shared<Shader> Create(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
	protected:
		ShaderType type;

		HashMap<String, ShaderResource> resourceInfo;
	};

}