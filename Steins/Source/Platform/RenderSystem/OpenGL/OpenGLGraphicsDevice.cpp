#include "SteinsPCH.h"
#include "OpenGLGraphicsDevice.h"
#include "Steins/Core/Window.h"

#include "glad/glad.h"

namespace Steins
{
	OpenGLGraphicsDevice::OpenGLGraphicsDevice()
	{
	}

	OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
	{

	}
	void OpenGLGraphicsDevice::Init()
	{
		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		STEINS_CORE_ASSERT(status, "Failed to initialize Glad!");

		STEINS_CORE_INFO("OpenGL Info:");
		STEINS_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		STEINS_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		STEINS_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}
	void OpenGLGraphicsDevice::Shutdown()
	{
	}
	void OpenGLGraphicsDevice::Render()
	{
	}
	void OpenGLGraphicsDevice::SwapBuffers()
	{

	}
	void OpenGLGraphicsDevice::Clear()
	{
	}

	void OpenGLGraphicsDevice::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
	}
}
