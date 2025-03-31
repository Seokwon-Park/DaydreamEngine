#pragma once

namespace Steins {

	class Shader
	{
	public:
		Shader(const std::string_view& _vertexSrc, const std::string& _pixelSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint64_t rendererID;
	};

}