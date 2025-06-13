#pragma once

#include "Steins/Graphics/Resources/Shader.h"
#include "glad/glad.h"

namespace Steins
{
	class OpenGLShader :public Shader
	{
	public:
		OpenGLShader(const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<uintptr_t>(shaderProgramID)); }

		//virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) override;
	private:
		void Compile(const std::string& _src);
	private:
		GLuint shaderProgramID;
	};
}

