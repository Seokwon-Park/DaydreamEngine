#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "VertexArray.h"

namespace Steins 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void* GetNativeHandle() = 0;
		//virtual void SetVertexArray(Shared<VertexArray> _vertexArray) { vertexArray = _vertexArray.get(); };

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) = 0;

		ShaderType GetType() const { return type; }

		static Shared<Shader> Create(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
	protected:
		ShaderType type;
		//VertexArray* vertexArray;
	};

}