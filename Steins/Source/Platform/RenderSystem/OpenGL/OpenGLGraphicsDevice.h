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
		virtual void SwapBuffers() override;

		std::string GetVersion() const { return version; }
	protected:

	private:
		GLFWwindow* nativeWindow;

		std::string version = "#version 410";
	};
}
