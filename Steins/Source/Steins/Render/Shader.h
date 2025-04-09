#pragma once

namespace Steins {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetMat4(const std::string& _name, const Matrix4x4& _value) = 0;

		static Shared<Shader> Create(const FilePath& _filepath);
		static Shared<Shader> Create(const std::string& _vertexSrc, const std::string& _pixelSrc);
	};

}