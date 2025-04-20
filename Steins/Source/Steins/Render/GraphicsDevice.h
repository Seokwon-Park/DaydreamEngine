#pragma once

#include "Base/MathTypes.h"
#include "Steins/Enum/RendererEnums.h"
#include "RendererAPI.h"

namespace Steins
{
	struct SwapChainDesc; 
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

		RendererAPIType GetAPI() const { return API; }

		static Unique<GraphicsDevice> Create(RendererAPIType _API);
		Shared<SwapChain> CreateSwapChain(SwapChainDesc* _desc, SteinsWindow* _window);
	protected:
		std::vector<Shared<SwapChain>> swapChains;
		RendererAPIType API = RendererAPIType::None;
	};
}
