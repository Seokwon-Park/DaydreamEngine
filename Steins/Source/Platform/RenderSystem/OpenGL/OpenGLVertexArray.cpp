#include "SteinsPCH.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Steins
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &rendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &rendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(rendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer)
	{

	}
	void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer)
	{
		glBindVertexArray(rendererID);
		_indexBuffer->Bind();
		indexBuffer = _indexBuffer;
	}
}