#pragma once

#include "WindowDefine.h"
#include "Steins/Event/Event.h"
#include "Steins/Graphics/Core/SwapChain.h"


namespace Steins
{
	struct WindowProps
	{
		std::string title;
		UInt32 width;
		UInt32 height;
		RendererAPIType rendererAPI;

		WindowProps(const std::string& _title = "Steins; Engine",
			UInt32 _width = 1280,
			UInt32 _height = 720)
			:title(_title), width(_width), height(_height), rendererAPI(RendererAPIType::None)
		{

		}
	};

	// why not glfw -> external .exe don't know about glfw3.h
	// Interface represending a desktop system based Window
	class SteinsWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		SteinsWindow() {};
		virtual ~SteinsWindow() {}

		virtual void OnUpdate() = 0;

		virtual UInt32 GetWidth() const = 0;
		virtual UInt32 GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& _callbackFn) = 0;
		virtual void SetVSync(bool _enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual int GetKeyState(int _key) const = 0;
		virtual int GetMouseState(int _mouse) const = 0;
		virtual Pair<Float32, Float32> GetMousePos() const = 0;
		virtual void* GetNativeWindow() const = 0;

		virtual void SetKeyState(int _key, int _state) = 0;
		virtual bool GetIsKeyDown(int _key) const = 0;
		virtual void SetKeyDown(int _key) = 0;
		virtual void OnUpdateKeyState() = 0;

		void SetSwapChain(Shared<SwapChain> _swapchain) { swapChain = _swapchain; }
		SwapChain* GetSwapChain() const { return swapChain.get(); }

		static Unique<SteinsWindow> Create(const WindowProps& _props = WindowProps());
	protected:
		Shared<SwapChain> swapChain = nullptr;
	private:
	};


}

