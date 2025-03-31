#include "SteinsPCH.h"
#include "OpenGLGraphicsDevice.h"
#include "glad/glad.h"

namespace Steins
{
	OpenGLGraphicsDevice::OpenGLGraphicsDevice(SteinsWindow* _window)
		:GraphicsDevice(_window)
	{
		nativeWindow = (GLFWwindow*)_window->GetNativeWindow();
	}

	OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
	{
		glfwMakeContextCurrent(nativeWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		STEINS_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLGraphicsDevice::Init()
	{
		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}
	void OpenGLGraphicsDevice::Shutdown()
	{
	}
	void OpenGLGraphicsDevice::Render()
	{
	}
	void OpenGLGraphicsDevice::SwapBuffers()
	{
		glfwSwapBuffers(nativeWindow);
	}
}
