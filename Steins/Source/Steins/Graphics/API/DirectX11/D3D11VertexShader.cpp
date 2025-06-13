#include "SteinsPCH.h"
#include "D3D11Shader.h"

namespace Steins
{
	D3D11VertexShader::D3D11VertexShader(D3D11RenderDevice* _device, const std::string& _src, const ShaderLoadMode& _mode)
		:D3D11Shader(_device, _src, ShaderType::Vertex, _mode)
	{
		device->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());

		ID3D11ShaderReflection* reflection = nullptr;
		HRESULT hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
		//if (FAILED(hr))

		// ���̴� �Է� �ñ״�ó ���� ���
		D3D11_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		// �Է� �Ķ���� ������ ������ ����
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

		// �� �Է� �Ķ���Ϳ� ���� ��ȸ
		for (UINT i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			reflection->GetInputParameterDesc(i, &paramDesc);

			// �Է� ��� ����
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// ������Ʈ Ÿ�� ����
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			inputLayoutDesc.push_back(elementDesc);
		}

		// �Է� ���̾ƿ� ����
		hr = device->GetDevice()->CreateInputLayout(
			inputLayoutDesc.data(),
			(UINT)inputLayoutDesc.size(),
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			inputLayout.GetAddressOf()
		);
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create inputlayout!");

		// ���ҽ� ����
		reflection->Release();
		shaderBlob.Reset();
		errorBlob.Reset();
	}

	void D3D11VertexShader::Bind() const
	{
		device->GetContext()->IASetInputLayout(inputLayout.Get());
		device->GetContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
	}
	void D3D11VertexShader::Unbind() const
	{
		device->GetContext()->VSSetShader(nullptr, nullptr, 0);
	}

}