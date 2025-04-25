#pragma once

#include "Steins/Render/VertexArray.h"
#include "D3D11GraphicsDevice.h"
namespace Steins
{
	class D3D11VertexArray : public VertexArray
	{
	public:
		D3D11VertexArray(D3D11GraphicsDevice* _device);
		virtual ~D3D11VertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer) override;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer) override;

		void CreateInputLayout(ID3DBlob* _vsBlob);
	private:
		D3D11GraphicsDevice* device;
		std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDescs;
		std::unordered_map<std::string, UInt32> semanticIndices;
		ComPtr<ID3D11InputLayout> inputLayout;
	};
}
