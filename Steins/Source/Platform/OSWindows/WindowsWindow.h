#pragma once

#include "Steins/Core/Window.h"
#include "Steins/Graphics/Core/RenderDevice.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Steins
{
	class WindowsWindow : public SteinsWindow
	{
	public:
		// constrcuter destructer
		WindowsWindow(const WindowProps& _props);
		virtual ~WindowsWindow() override;

		void OnUpdate() override;

		inline UInt32 GetWidth() const override { return windowData.width; }
		inline UInt32 GetHeight() const override { return windowData.height; }

		inline void SetEventCallback(const EventCallbackFn& _callbackFn) override { windowData.eventCallbackFn = _callbackFn; }
		void SetVSync(bool _enabled) override;
		bool IsVSync() const override;

		inline int GetKeyState(int _key) const override { return windowData.keyStates[_key]; }
		inline int GetMouseState(int _mousebutton) const override { return glfwGetMouseButton(glfwWindow, _mousebutton); }

		inline Pair<Float32, Float32> GetMousePos() const override
		{
			double xpos, ypos;
			glfwGetCursorPos(glfwWindow, &xpos, &ypos);
			return MakePair<Float32, Float32>(static_cast<Float32>(xpos), static_cast<Float32>(ypos));
		}
		void* GetNativeWindow() const override { return glfwWindow; }

		inline void SetKeyState(int _key, int _state)override { windowData.keyStates[_key] = _state; }
		// �� �Լ��� window���� releas
		inline bool GetIsKeyDown(int _key) const override { return windowData.keyDownChecker[_key]; }
		inline void SetKeyDown(int _key) override { windowData.keyDownChecker[_key] = true; }
		void OnUpdateKeyState() override;

	private:
		virtual void Init(const WindowProps& _props);
		virtual void Shutdown();

		GLFWwindow* glfwWindow = nullptr;

		HWND windowHandle = nullptr;

		struct WindowData
		{
			std::string title;
			UInt32 width = 0;
			UInt32 height = 0;
			bool isVSync;
			EventCallbackFn eventCallbackFn;
			Array<UInt16> keyStates;
			Array<bool> keyDownChecker;
		};
		WindowData windowData;
	};
}

