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

	}
	void OpenGLGraphicsDevice::Init()
	{
		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwMakeContextCurrent(nativeWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		STEINS_CORE_ASSERT(status, "Failed to initialize Glad!");

		//STEINS_CORE_INFO("OpenGL Info:");
		//STEINS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//STEINS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//STEINS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
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
