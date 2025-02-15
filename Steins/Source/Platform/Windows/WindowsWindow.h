#pragma once

#include "Steins/Window.h"

namespace Steins
{
	class WindowsWindow : public SteinsWindow
	{
	public:
		// constrcuter destructer
		WindowsWindow(const WindowProps& _props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		
		inline uint32 GetWidth() const override { return windowData.width; }
		inline uint32 GetHeight() const override { return windowData.height; }
		
		inline void SetEventCallback(const EventCallbackFn& _callbackFn) override { windowData.eventCallbackFn = _callbackFn; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		

		LRESULT MsgProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	private:
		virtual void Init(const WindowProps& _props);
		virtual void Shutdown();
		bool InitWindow();
		virtual void CreateKeyCodeTable() override;

		HINSTANCE windowInstance;
		HWND windowHandle;

		struct WindowData
		{
			std::string title;
			uint32 width, height;
			bool isVSync;
			EventCallbackFn eventCallbackFn;
		};
		WindowData windowData;
		
		std::map<uint16, uint16> keycodes;
		std::map<uint16, uint16> scancodes;
	};
}

