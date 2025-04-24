#pragma once

#include "Steins/Render/VertexArray.h"

namespace Steins
{
	class VulkanVertexArray : public VertexArray
	{
	public:
		// constrcuter destructer
		VulkanVertexArray();
		virtual ~VulkanVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer) override;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer) override;

	private:

	};
}

