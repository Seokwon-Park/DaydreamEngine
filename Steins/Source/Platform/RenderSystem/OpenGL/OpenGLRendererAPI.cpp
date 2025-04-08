#include "SteinsPCH.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Steins
{
	void OpenGLRendererAPI::SetClearColor(const Color& _color)
	{
		glClearColor(_color.r, _color.g, _color.b, _color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, (void*)(_startIndex * sizeof(uint32_t)), _baseVertex);
	}
}