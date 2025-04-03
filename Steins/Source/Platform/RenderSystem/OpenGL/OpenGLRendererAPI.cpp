#include "SteinsPCH.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Steins
{
	void OpenGLRendererAPI::SetClearColor(const Color& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}