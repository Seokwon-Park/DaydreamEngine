#include "SteinsPCH.h"
#include "D3D11VertexArray.h"
#include "D3D11Buffer.h"

#include "D3D11GraphicsDevice.h"

namespace Steins
{
	static DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type)
	{
		switch (type)
		{
		case Steins::ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
		case Steins::ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case Steins::ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case Steins::ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case Steins::ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
		case Steins::ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
		case Steins::ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
		case Steins::ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
		case Steins::ShaderDataType::Mat3x3:     return DXGI_FORMAT_UNKNOWN;
		case Steins::ShaderDataType::Mat4x4:     return DXGI_FORMAT_UNKNOWN;
		case Steins::ShaderDataType::Bool:     return DXGI_FORMAT_UNKNOWN;
		}

		STEINS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}
	D3D11VertexArray::D3D11VertexArray()
	{
	}
	D3D11VertexArray::~D3D11VertexArray()
	{
	}
	void D3D11VertexArray::Bind() const
	{
		D3D11GraphicsDevice::GetContext()->IASetInputLayout(inputLayout.Get());
		for (auto vertexBuffer : vertexBuffers)
		{
			vertexBuffer->Bind();
		}
		indexBuffer->Bind();
	}
	void D3D11VertexArray::Unbind() const
	{
		D3D11GraphicsDevice::GetContext()->IASetInputLayout(nullptr);
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
			desc.Format = ShaderDataTypeToDXGIFormat(elem.type);
			desc.InputSlot = (UINT)vertexBuffers.size();
			desc.AlignedByteOffset = elem.offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			layoutDescs.push_back(desc);
		}
		
		static_cast<D3D11VertexBuffer*>(_vertexBuffer.get())->SetSlotNum((UInt32)vertexBuffers.size());
		vertexBuffers.push_back(_vertexBuffer);
	}
	void D3D11VertexArray::SetIndexBuffer(const Shared<IndexBuffer>& _indexBuffer)
	{
		indexBuffer = _indexBuffer;
	}
	void D3D11VertexArray::CreateInputLayout(ID3DBlob* _vsBlob)
	{
		D3D11GraphicsDevice::GetDevice()->CreateInputLayout(layoutDescs.data(), (UINT)layoutDescs.size(), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), inputLayout.GetAddressOf());
	}
}