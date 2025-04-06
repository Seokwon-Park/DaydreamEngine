#pragma once

namespace Steins {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shared<Shader> Create(const std::string_view& _vertexSrc, const std::string& _pixelSrc);
	};

}