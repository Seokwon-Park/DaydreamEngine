#pragma once

#include "Steins/Render/VertexArray.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12VertexArray : public VertexArray
	{
	public:
		virtual ~D3D12VertexArray() override;
		void Bind() const override;

		void Unbind() const override;

		void AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer) override;

		void SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer) override;
	private:
		D3D12GraphicsDevice* device;
	};
}