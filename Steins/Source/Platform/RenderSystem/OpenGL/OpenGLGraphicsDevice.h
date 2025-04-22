#pragma once
#include "Steins/Render/GraphicsDevice.h"
#include "GLFW/glfw3.h"
//#include "glad/glad.h"

namespace Steins
{
	class OpenGLGraphicsDevice: public GraphicsDevice
	{
	public:
		OpenGLGraphicsDevice();
		~OpenGLGraphicsDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<VertexBuffer> CreateVertexBuffer(Float32* _vertices, UInt32 _size) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<Framebuffer> CreateFramebuffer(FramebufferSpecification _spec)override;
		virtual Shared<PipelineState> CreatePipelineState(PipelineStateDesc _desc)override;
		virtual Shared<Shader> CreateShader(const FilePath& _filepath, const ShaderType& _type)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type)override;
		virtual Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path)override;

		std::string GetVersion() const { return version; }
	protected:

	private:
		GLFWwindow* nativeWindow;

		std::string version = "#version 410";
	};
}
