#include "SteinsPCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

namespace Steins {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(Float32* _vertices, UInt32 _size, const BufferLayout& _layout)
	{
		layout = _layout;
		glCreateBuffers(1, &rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ARRAY_BUFFER, _size, _vertices, GL_STATIC_DRAW);
	}


	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(UInt32* _indices, UInt32 _indexCount)
	{
		indexCount = _indexCount;
		glCreateBuffers(1, &rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(UInt32), _indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////////
// ConstantBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	OpenGLConstantBuffer::OpenGLConstantBuffer(const void* _data, UInt32 _size)
	{
		glCreateBuffers(1, &bufferID);
		glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
		glBufferData(GL_UNIFORM_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	OpenGLConstantBuffer::~OpenGLConstantBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	void OpenGLConstantBuffer::Bind(UInt32 _slot) const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferID);
	}
	void OpenGLConstantBuffer::Update(const void* _data, UInt32 _size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, _size, _data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}
}