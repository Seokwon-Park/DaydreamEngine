#pragma once

#include <d3d12.h>

namespace Daydream
{
	class D3D12ResourceState
	{
	public:
		virtual ~D3D12ResourceState() = default;

		D3D12_RESOURCE_STATES GetCurrentState() const { return currentState; }
		void SetCurrentState(D3D12_RESOURCE_STATES state) { currentState = state; }
	private:
		D3D12_RESOURCE_STATES currentState;
	};
}
