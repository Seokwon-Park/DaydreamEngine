#pragma once
#include "Steins/Render/GraphicsDevice.h"
#include "GLFW/glfw3.h"
//#include "glad/glad.h"

namespace Steins
{
	class OpenGLGraphicsDevice: public GraphicsDevice
	{
	public:
		OpenGLGraphicsDevice(SteinsWindow* _window);
		~OpenGLGraphicsDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;
	protected:

	private:
		GLFWwindow* nativeWindow;
	};
}
