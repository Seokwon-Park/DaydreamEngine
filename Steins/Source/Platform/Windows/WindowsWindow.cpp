#include "SteinsPCH.h"
#include "WindowsWindow.h"

#include "Steins/Event/KeyEvent.h"
#include "Steins/Event/MouseEvent.h"
#include "Steins/Event/ApplicationEvent.h"

namespace Steins
{
	namespace
	{
		WindowsWindow* windowsBase = nullptr;
		// RegisterClassEX()�� ������ ��ϵ� �ݹ��Լ�
		LRESULT WINAPI WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
		{
			// g_appBase�� �̿��ؼ� ���������� ��� �Լ� ȣ��
			return windowsBase->MsgProc(_hwnd, _msg, _wParam, _lParam);
		}
	}

	Window* Window::Create(const WindowProps& _props)
	{
		return new WindowsWindow(_props);
	}
	WindowsWindow::WindowsWindow(const WindowProps& _props)
	{
		Init(_props);
	}
	WindowsWindow::~WindowsWindow()
	{
	}
	void WindowsWindow::Init(const WindowProps& _props)
	{
		windowData.title = _props.title;
		windowData.width = _props.width;
		windowData.height = _props.height;
		std::wstring title(windowData.title.begin(), windowData.title.end());
		STEINS_CORE_INFO("Create Window {0} ({1}, {2})", _props.title, _props.width, _props.height);

		bool result = InitWindow();
		if (false == result)
		{
			STEINS_CORE_ASSERT(false, "Failed to Initialize Window");
		}

		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&windowData));

	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow(windowHandle);
	}
	void WindowsWindow::OnUpdate()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		windowData.isVSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return windowData.isVSync;
	}
	bool WindowsWindow::InitWindow()
	{
		windowInstance = GetModuleHandle(nullptr);
		std::wstring title(windowData.title.begin(), windowData.title.end());

		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0L;
		wc.cbWndExtra = 0L;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"SteinsEngine"; // lpszClassName, L-string;
		wc.hIconSm = NULL;

		// The RegisterClass function has been superseded by the RegisterClassEx function.
		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
		if (!RegisterClassEx(&wc)) {
			std::cout << "RegisterClassEx() failed.\n";
			return false;
		}
		// ���ϴ� ũ���� �ػ�(���� ����)
		RECT windowRect = { 0, 0, static_cast<LONG>(windowData.width), static_cast<LONG>(windowData.height) };
		// �ʿ��� ������ ũ��(�ػ�) ��� (���ٸ� ����)
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
		// �����츦 ���鶧 ������ ����� windowRect ���
		windowHandle = CreateWindow(
			wc.lpszClassName,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			//�������� ���� ��ġ
			100,                // ������ ���� ����� x ��ǥ
			100,                // ������ ���� ����� y ��ǥ
			// ���ٸ� ������ ���� �������� ũ�� ���
			windowRect.right - windowRect.left, // ������ ���� ���� �ػ�
			windowRect.bottom - windowRect.top, // ������ ���� ���� �ػ�
			NULL, NULL, wc.hInstance, NULL);

		if (!windowHandle) {
			std::cout << "CreateWindow() failed.\n";
			return false;
		}
		// ������ ǥ��
		ShowWindow(windowHandle, SW_SHOWDEFAULT);
		UpdateWindow(windowHandle);
		return true;
	}

	LRESULT WindowsWindow::MsgProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
	{
		//if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		//	return true;
		WindowData* windowDataPtr = reinterpret_cast<WindowData*>(GetWindowLongPtr(_hwnd, GWLP_USERDATA));

		switch (_msg) {
		case WM_SIZE:
		{
			const int width = LOWORD(_lParam);
			const int height = HIWORD(_lParam);

			windowDataPtr->width = width;
			windowDataPtr->height = height;

			WindowResizeEvent event(width, height);
			windowDataPtr->eventCallbackFn(event);
			break;
		}
		case WM_KEYDOWN:
		{
			KeyPressedEvent event(static_cast<int>(_wParam), 0);
			windowDataPtr->eventCallbackFn(event);
			break;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent event(static_cast<int>(_wParam));
			windowDataPtr->eventCallbackFn(event);
			break;
		}

		case WM_SYSCOMMAND:
			if ((_wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_MOUSEMOVE:
			//std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
			break;
		case WM_LBUTTONDOWN:
			std::cout << "WM_LBUTTONUP Left mouse button down\n";

			break;
		case WM_LBUTTONUP:
			std::cout << "WM_LBUTTONUP Left mouse button up\n";

			break;
		case WM_RBUTTONDOWN:
			std::cout << "WM_RBUTTONUP Right mouse button\n";

			break;
		case WM_RBUTTONUP:
			std::cout << "WM_RBUTTONUP Right mouse button\n";

			break;
		case WM_KEYDOWN:
			std::cout << "WM_KEYDOWN " << (int)_wParam << std::endl;

			break;
		case WM_KEYUP:

			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}
		return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
	}
}
