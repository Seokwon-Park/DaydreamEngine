#pragma once

#include "Base/MathTypes.h"
#include "Steins/Enum/RendererEnums.h"
#include "RendererAPI.h"

namespace Steins
{
	struct SwapChainSpecification; 
	class SwapChain;
	class SteinsWindow;

	class GraphicsDevice
	{
	public:
		virtual ~GraphicsDevice() {};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Render() = 0;
		virtual void SwapBuffers() = 0;

		template <typename DeviceType>
		DeviceType* Get()
		{
			return SafeCast<DeviceType>(this);
		}

		inline RendererAPIType GetAPI() const { return API; }
		inline SwapChain* GetSwapChain(UInt32 _index)
		{
			STEINS_CORE_ASSERT(swapChains.size() > _index, "Not valid index!");
			return swapChains[_index];
		};
		inline void AddSwapChain(SwapChain* _swapChain) { swapChains.push_back(_swapChain); }

		static Unique<GraphicsDevice> Create(RendererAPIType _API);
		Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window);
	protected:
		std::vector<SwapChain*> swapChains;
		RendererAPIType API = RendererAPIType::None;
	};
}
