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

	// Interface represending a desktop system based Window
	class SteinsWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		struct WindowCallbacks
		{
			std::function<void(SteinsWindow*, int, int)> inputKeyFn;
			std::function<void(SteinsWindow*, int, int)> mouseButtonFn;
			std::function<void(SteinsWindow*, double, double)> mouseScrollFn;
			std::function<void(SteinsWindow*, double, double)> cursorPosFn;
		};

		SteinsWindow();
		virtual ~SteinsWindow() {}

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& _callbackFn) = 0;
		virtual void SetVSync(bool _enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void CreateKeyCodeTable()= 0;

		void SetUserData(void* _userData) { userData = _userData; }
		void* GetUserData() const { return userData; }
		void WindowInputKey(int _key, int _action);
		void WindowMouseClick(int _button, int _action);
		void WindowMouseScroll(double _xOffset, double _yOffset);
		void WindowCursorPos(double _xPos, double _yPos);

		inline void SetInputKeyCallback(std::function<void(SteinsWindow*, int, int)> _inputKeyFn) { callbacks.inputKeyFn = _inputKeyFn; }
		inline void SetMouseButtonCallback(std::function<void(SteinsWindow*, int, int)> _mouseButtonFn) { callbacks.mouseButtonFn = _mouseButtonFn; }
		inline void SetScrollCallback(std::function<void(SteinsWindow*, double, double)> _mouseScrollFn){ callbacks.mouseScrollFn = _mouseScrollFn; }
		inline void SetCursorPosCallback(std::function<void(SteinsWindow*, double, double)> _mouseMoveFn) { callbacks.cursorPosFn = _mouseMoveFn; }

		
		inline int GetKeyState(int _key) { return keyStates[_key]; }

		inline void ClearKeyStates() 
		{ 
			for (uint16& state : keyStates)
			{
				if (state == STEINS_RELEASE)
				{
					state = STEINS_IDLE;
				}
			}
		}

		static Unique<SteinsWindow> Create(const WindowProps& _props = WindowProps());
	protected:
		std::vector<uint16> keyStates;
		std::vector<uint16> mouseStates;
		WindowCallbacks callbacks;
	private:
		void* userData = nullptr;
	};


}

