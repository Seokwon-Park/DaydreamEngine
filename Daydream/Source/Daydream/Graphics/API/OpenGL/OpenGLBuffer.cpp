#include "DaydreamPCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "OpenGLUtility.h"

namespace Daydream
{

	OpenGLGPUBuffer::OpenGLGPUBuffer(const BufferDesc& _desc)
		:GPUBuffer(_desc)
	{
		glCreateBuffers(1, &bufferID);

		// 2. MemoryUsage를 OpenGL의 힌트 플래그로 변환
		GLenum glUsage = GraphicsUtility::OpenGL::ConvertToOpenGLMemoryUsage(_desc.memoryUsage);
		glNamedBufferData(bufferID, _desc.size, nullptr, glUsage);
	}
	OpenGLGPUBuffer::~OpenGLGPUBuffer()
	{
		glDeleteBuffers(1, &bufferID);
	}
	void OpenGLGPUBuffer::UpdateData(const void* _data, UInt32 _size)
	{
		glNamedBufferSubData(bufferID, 0, _size, _data);
	}

//	/////////////////////////////////////////////////////////////////////////////
//	// VertexBuffer /////////////////////////////////////////////////////////////
//	/////////////////////////////////////////////////////////////////////////////
//
//	OpenGLVertexBuffer::OpenGLVertexBuffer(UInt32 _size, UInt32 _stride, const void* _vertices)
//	{
//		stride = _stride;
//		glCreateBuffers(1, &bufferID);
//		if (_vertices)
//		{
//			glNamedBufferData(bufferID, _size, _vertices, GL_STATIC_DRAW);
//		}
//		else
//		{
//			glNamedBufferData(bufferID, _size, nullptr, GL_DYNAMIC_DRAW);
//		}
//	}
//
//	OpenGLVertexBuffer::~OpenGLVertexBuffer()
//	{
//		glDeleteBuffers(1, &bufferID);
//	}
//
//	void OpenGLVertexBuffer::Bind() const
//	{
//		GLint currentVAO = 0;
//		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
//		glVertexArrayVertexBuffer(currentVAO, 0, bufferID, 0, stride);
//	}
//
//	void OpenGLVertexBuffer::Unbind() const
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//	}
//
//	void OpenGLVertexBuffer::SetData(const void* _data, UInt32 _dataSize)
//	{
//		glNamedBufferSubData(bufferID, 0, _dataSize, _data);
//	}
//
//	/////////////////////////////////////////////////////////////////////////////
//	// IndexBuffer //////////////////////////////////////////////////////////////
//	/////////////////////////////////////////////////////////////////////////////
//
//	OpenGLIndexBuffer::OpenGLIndexBuffer(const UInt32* _indices, UInt32 _indexCount)
//	{
//		indexCount = _indexCount;
//		glCreateBuffers(1, &bufferID);
//		glNamedBufferData(bufferID, _indexCount * sizeof(UInt32), _indices, GL_STATIC_DRAW);
//	}
//
//	OpenGLIndexBuffer::~OpenGLIndexBuffer()
//	{
//		glDeleteBuffers(1, &bufferID);
//	}
//
//	void OpenGLIndexBuffer::Bind() const
//	{
//		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
//		GLint currentVAO = 0; // 결과를 저장할 변수
//		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
//		glVertexArrayElementBuffer(currentVAO, bufferID);
//	}
//
//	void OpenGLIndexBuffer::Unbind() const
//	{
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//
//	/////////////////////////////////////////////////////////////////////////////
//// ConstantBuffer //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//	OpenGLConstantBuffer::OpenGLConstantBuffer(UInt32 _size)
//	{
//		glCreateBuffers(1, &bufferID);
//		glNamedBufferData(bufferID, _size, nullptr, GL_DYNAMIC_DRAW);
//	}
//
//	OpenGLConstantBuffer::~OpenGLConstantBuffer()
//	{
//		glDeleteBuffers(1, &bufferID);
//	}
//
//	//void OpenGLConstantBuffer::Bind(UInt32 _slot) const
//	//{
//	//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferID);
//
//
//	void OpenGLConstantBuffer::UpdateData(const void* _data, UInt32 _size)
//	{
//		glNamedBufferSubData(bufferID, 0, _size, _data);
//	}

}