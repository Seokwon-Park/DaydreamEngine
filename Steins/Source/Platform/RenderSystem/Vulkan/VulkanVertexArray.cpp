#include "SteinsPCH.h"
#include "VulkanVertexArray.h"

namespace Steins
{
	VulkanVertexArray::VulkanVertexArray()
	{
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
	}

	void VulkanVertexArray::Bind() const
	{
		for (auto vb : vertexBuffers)
		{
			vb->Bind();
		}
		indexBuffer->Bind();
	}

	void VulkanVertexArray::Unbind() const
	{
	}

	void VulkanVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer)
	{
		_vertexBuffer->SetSlot(vertexBuffers.size());
		vertexBuffers.push_back(_vertexBuffer);
	}

	void VulkanVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer)
	{
		indexBuffer = _indexBuffer;
	}
}
