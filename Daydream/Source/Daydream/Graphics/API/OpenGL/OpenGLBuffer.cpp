#include "DaydreamPCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

namespace Daydream {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(UInt32 _bufferSize, UInt32 _stride)
	{
		stride = _stride;
		glCreateBuffers(1, &bufferID);
		glNamedBufferData(bufferID, _bufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* _vertices, UInt32 _size, UInt32 _stride)
	{
		stride = _stride;
		glCreateBuffers(1, &bufferID);
		glNamedBufferData(bufferID, _size, _vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		//glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		GLint currentVAO = 0; // 결과를 저장할 변수
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
		glVertexArrayVertexBuffer(currentVAO, 0, bufferID, 0, stride);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(void* _data, UInt32 _dataSize)
	{
		glNamedBufferSubData(bufferID, 0, _dataSize, _data);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(UInt32* _indices, UInt32 _indexCount)
	{
		indexCount = _indexCount;
		glCreateBuffers(1, &bufferID);
		glNamedBufferData(bufferID, _indexCount * sizeof(UInt32), _indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		GLint currentVAO = 0; // 결과를 저장할 변수
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
		glVertexArrayElementBuffer(currentVAO, bufferID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////////
// ConstantBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	OpenGLConstantBuffer::OpenGLConstantBuffer(UInt32 _size)
	{
		glCreateBuffers(1, &bufferID);
		glNamedBufferData(bufferID, _size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLConstantBuffer::~OpenGLConstantBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}

	//void OpenGLConstantBuffer::Bind(UInt32 _slot) const
	//{
	//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferID);

	
	void OpenGLConstantBuffer::Update(const void* _data, UInt32 _size)
	{
		glNamedBufferSubData(bufferID, 0, _size, _data);
	}
}