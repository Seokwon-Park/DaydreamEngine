#pragma once

#include "SteinsPCH.h"

#include "Steins/Event/Event.h"
#include "WindowDefine.h"


namespace Steins
{
	struct WindowProps
	{
		std::string title;
		uint32 width;
		uint32 height;

		WindowProps(const std::string& _title = "Steins; Engine",
			uint32 _width = 1280,
			uint32 _height = 720)
			:title(_title), width(_width), height(_height)
		{

		}
	};

	// why not glfw -> external .exe don't know about glfw3.h
	// Interface represending a desktop system based Window
	class SteinsWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		SteinsWindow();
		virtual ~SteinsWindow() {}

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& _callbackFn) = 0;
		virtual void SetVSync(bool _enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual int GetKeyState(int _key) const = 0;
		virtual int GetMouseState(int _mouse) const = 0;
		virtual Pair<float32, float32> GetMousePos() const = 0;
		virtual void* GetNativeWindow() const = 0;

		virtual void SetKeyState(int _key, int _state) = 0;
		virtual bool GetIsKeyDown(int _key) const = 0;
		virtual void SetKeyDown(int _key) = 0;
		virtual void OnUpdateKeyState() = 0;

		static Unique<SteinsWindow> Create(const WindowProps& _props = WindowProps());
	protected:
	private:
	};


}

