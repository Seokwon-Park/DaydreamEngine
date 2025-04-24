#include "SteinsPCH.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Steins
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Steins::ShaderDataType::Float:    return GL_FLOAT;
		case Steins::ShaderDataType::Float2:   return GL_FLOAT;
		case Steins::ShaderDataType::Float3:   return GL_FLOAT;
		case Steins::ShaderDataType::Float4:   return GL_FLOAT;
		case Steins::ShaderDataType::Int:      return GL_INT;
		case Steins::ShaderDataType::Int2:     return GL_INT;
		case Steins::ShaderDataType::Int3:     return GL_INT;
		case Steins::ShaderDataType::Int4:     return GL_INT;
		case Steins::ShaderDataType::Mat3x3:     return GL_FLOAT;
		case Steins::ShaderDataType::Mat4x4:     return GL_FLOAT;
		case Steins::ShaderDataType::Bool:     return GL_BOOL;
		}

		STEINS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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
		STEINS_CORE_ASSERT(_vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(rendererID);
		_vertexBuffer->Bind();
		
		uint32_t index = 0;
		const BufferLayout& layout = _vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(inputDataIndex);
			glVertexAttribPointer(inputDataIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				&element.offset);
			inputDataIndex++;
		}

		vertexBuffers.push_back(_vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer)
	{
		glBindVertexArray(rendererID);
		_indexBuffer->Bind();
		indexBuffer = _indexBuffer;
	}
}