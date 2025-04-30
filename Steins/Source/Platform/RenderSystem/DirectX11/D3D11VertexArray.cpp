#include "SteinsPCH.h"
#include "D3D11VertexArray.h"
#include "D3D11Buffer.h"

#include "D3D11GraphicsDevice.h"
#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{

	D3D11VertexArray::D3D11VertexArray(D3D11GraphicsDevice* _device)
	{
		device = _device;
	}
	D3D11VertexArray::~D3D11VertexArray()
	{
	}
	void D3D11VertexArray::Bind() const
	{
		//device->GetContext()->IASetInputLayout(inputLayout.Get());
		for (auto vertexBuffer : vertexBuffers)
		{
			vertexBuffer->Bind();
		}
		indexBuffer->Bind();
	}
	void D3D11VertexArray::Unbind() const
	{
		//D3D11GraphicsDevice::GetContext()->IASetInputLayout(nullptr);
		for (auto vertexBuffer : vertexBuffers)
		{
			vertexBuffer->Unbind();
		}
		indexBuffer->Unbind();
	}
	void D3D11VertexArray::AddVertexBuffer(const Shared<VertexBuffer>& _vertexBuffer)
	{
		const BufferLayout& vertexLayout = _vertexBuffer->GetLayout();

		for (BufferElement elem : vertexLayout)
		{
			D3D11_INPUT_ELEMENT_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.SemanticName = elem.semantic.c_str();
			desc.SemanticIndex = semanticIndices[elem.semantic];
			desc.Format = GraphicsUtil::ShaderDataTypeToDXGIFormat(elem.type);
			desc.InputSlot = (UINT)vertexBuffers.size();
			desc.AlignedByteOffset = elem.offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			layoutDescs.push_back(desc);
		}
		
		_vertexBuffer->SetSlot((UInt32)vertexBuffers.size());
		vertexBuffers.push_back(_vertexBuffer);
	}
	void D3D11VertexArray::SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer)
	{
		indexBuffer = _indexBuffer;
	}
	void D3D11VertexArray::CreateInputLayout(ID3DBlob* _vsBlob)
	{
		device->GetDevice()->CreateInputLayout(layoutDescs.data(), (UINT)layoutDescs.size(), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), inputLayout.GetAddressOf());
	}
}