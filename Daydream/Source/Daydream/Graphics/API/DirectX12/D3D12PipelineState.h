#pragma once

#include "Daydream/Graphics/Resources/PipelineState.h"

#include "D3D12RenderDevice.h"

namespace Daydream
{
	class D3D12PipelineState : public PipelineState
	{
	public:
		// Constrcuter Destructer
		D3D12PipelineState(D3D12RenderDevice* _device, PipelineStateDesc _desc);
		virtual ~D3D12PipelineState();

		virtual void Bind() const override;
		//virtual Shared<Material> CreateMaterial() override;

		ID3D12RootSignature* GetID3D12RootSignature() const { return rootSignature.Get(); }
		ID3D12PipelineState* GetID3D12PipelineState() const { return pipeline.Get(); }
		UInt32 GetDescriptorTableIndex(String _resourceName);
	protected:
		 
	private:
		D3D12RenderDevice* device;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipeline;

		HashMap<String, UInt32> descriptorTable;

		Array<D3D12_ROOT_PARAMETER> rootParameters;
		Array<D3D12_DESCRIPTOR_RANGE> srvRanges;
		Array<D3D12_DESCRIPTOR_RANGE> samplerRanges;
	};
}
