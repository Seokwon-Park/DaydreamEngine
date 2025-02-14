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
		// RegisterClassEX()에 실제로 등록될 콜백함수
		LRESULT WINAPI WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
		{
			// g_appBase를 이용해서 간접적으로 멤버 함수 호출
			return windowsBase->MsgProc(_hwnd, _msg, _wParam, _lParam);
		}
	}

	WindowsWindow::WindowsWindow(const WindowProps& _props)
		:SteinsWindow()
	{
		Init(_props);
	}
	WindowsWindow::~WindowsWindow()
	{
	}
	void WindowsWindow::Init(const WindowProps& _props)
	{
		windowsBase = this;

		windowData.title = _props.title;
		windowData.width = _props.width;
		windowData.height = _props.height;
		std::wstring title(windowData.title.begin(), windowData.title.end());
		STEINS_CORE_INFO("Create Window {0} ({1}, {2})", _props.title, _props.width, _props.height);

		SetUserData(&windowData);

		CreateKeyTables();

		SetInputKeyCallback([&](SteinsWindow* _window, int _key, int _action)
			{
				WindowData& data = *(WindowData*)GetUserData();

				switch (_action)
				{
				case STEINS_PRESS:
				{
					KeyPressedEvent event(_key, 0);
					data.eventCallbackFn(event);
					break;
				}
				case STEINS_RELEASE:
				{
					KeyReleasedEvent event(_key);
					data.eventCallbackFn(event);
					break;
				}
				case STEINS_REPEAT:
				{
					KeyPressedEvent event(_key, true);
					data.eventCallbackFn(event);
					break;
				}
				}
			});

		SetMouseButtonCallback([&](SteinsWindow* _window, int _button, int _action)
			{
				WindowData& data = *(WindowData*)GetUserData();

				switch (_action)
				{
				case STEINS_PRESS:
				{
					MouseButtonPressedEvent event(_button);
					data.eventCallbackFn(event);
					break;
				}
				case STEINS_RELEASE:
				{
					MouseButtonReleasedEvent event(_button);
					data.eventCallbackFn(event);
					break;
				}
				}
			});

		SetScrollCallback([&](SteinsWindow* _window, double _xOffset, double _yOffset)
			{
				WindowData& data = *(WindowData*)GetUserData();

				MouseScrolledEvent event((float)_xOffset, (float)_yOffset);
				data.eventCallbackFn(event);
			});

		SetCursorPosCallback([&](SteinsWindow* _window, double _xPos, double _yPos)
			{
				WindowData& data = *(WindowData*)GetUserData();

				MouseMovedEvent event((float)_xPos, (float)_yPos);
				data.eventCallbackFn(event);
			});

		bool result = InitWindow();
		if (false == result)
		{
			STEINS_CORE_ASSERT(false, "Failed to Initialize Window");
		}
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
		wc.hInstance = windowInstance;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"SteinsEngine"; // lpszClassName, L-string;
		wc.hIconSm = NULL;

		// The RegisterClass function has been superseded by the RegisterClassEx function.
		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
		if (!RegisterClassEx(&wc))
		{
			std::cout << "RegisterClassEx() failed.\n";
			return false;
		}
		// 원하는 크기의 해상도(툴바 제외)
		RECT windowRect = { 0, 0, static_cast<LONG>(windowData.width), static_cast<LONG>(windowData.height) };
		// 필요한 윈도우 크기(해상도) 계산 (툴바를 포함)
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
		// 윈도우를 만들때 위에서 계산한 windowRect 사용
		windowHandle = CreateWindow(
			wc.lpszClassName,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			//윈도우의 생성 위치
			100,                // 윈도우 좌측 상단의 x 좌표
			100,                // 윈도우 좌측 상단의 y 좌표
			// 툴바를 포함한 실제 윈도우의 크기 계산
			windowRect.right - windowRect.left, // 윈도우 가로 방향 해상도
			windowRect.bottom - windowRect.top, // 윈도우 세로 방향 해상도
			NULL, NULL, wc.hInstance, NULL);

		if (windowHandle == nullptr)
		{
			std::cout << "CreateWindow() failed.\n";
			return false;
		}
		// 윈도우 표시
		ShowWindow(windowHandle, SW_SHOWDEFAULT);
		UpdateWindow(windowHandle);
		return true;
	}

	void WindowsWindow::CreateKeyTables()
	{
		int scancode;

		keycodes[0x00B] = STEINS_KEY_0;
		keycodes[0x002] = STEINS_KEY_1;
		keycodes[0x003] = STEINS_KEY_2;
		keycodes[0x004] = STEINS_KEY_3;
		keycodes[0x005] = STEINS_KEY_4;
		keycodes[0x006] = STEINS_KEY_5;
		keycodes[0x007] = STEINS_KEY_6;
		keycodes[0x008] = STEINS_KEY_7;
		keycodes[0x009] = STEINS_KEY_8;
		keycodes[0x00A] = STEINS_KEY_9;
		keycodes[0x01E] = STEINS_KEY_A;
		keycodes[0x030] = STEINS_KEY_B;
		keycodes[0x02E] = STEINS_KEY_C;
		keycodes[0x020] = STEINS_KEY_D;
		keycodes[0x012] = STEINS_KEY_E;
		keycodes[0x021] = STEINS_KEY_F;
		keycodes[0x022] = STEINS_KEY_G;
		keycodes[0x023] = STEINS_KEY_H;
		keycodes[0x017] = STEINS_KEY_I;
		keycodes[0x024] = STEINS_KEY_J;
		keycodes[0x025] = STEINS_KEY_K;
		keycodes[0x026] = STEINS_KEY_L;
		keycodes[0x032] = STEINS_KEY_M;
		keycodes[0x031] = STEINS_KEY_N;
		keycodes[0x018] = STEINS_KEY_O;
		keycodes[0x019] = STEINS_KEY_P;
		keycodes[0x010] = STEINS_KEY_Q;
		keycodes[0x013] = STEINS_KEY_R;
		keycodes[0x01F] = STEINS_KEY_S;
		keycodes[0x014] = STEINS_KEY_T;
		keycodes[0x016] = STEINS_KEY_U;
		keycodes[0x02F] = STEINS_KEY_V;
		keycodes[0x011] = STEINS_KEY_W;
		keycodes[0x02D] = STEINS_KEY_X;
		keycodes[0x015] = STEINS_KEY_Y;
		keycodes[0x02C] = STEINS_KEY_Z;

		keycodes[0x028] = STEINS_KEY_APOSTROPHE;
		keycodes[0x02B] = STEINS_KEY_BACKSLASH;
		keycodes[0x033] = STEINS_KEY_COMMA;
		keycodes[0x00D] = STEINS_KEY_EQUAL;
		keycodes[0x029] = STEINS_KEY_GRAVE_ACCENT;
		keycodes[0x01A] = STEINS_KEY_LEFT_BRACKET;
		keycodes[0x00C] = STEINS_KEY_MINUS;
		keycodes[0x034] = STEINS_KEY_PERIOD;
		keycodes[0x01B] = STEINS_KEY_RIGHT_BRACKET;
		keycodes[0x027] = STEINS_KEY_SEMICOLON;
		keycodes[0x035] = STEINS_KEY_SLASH;
		keycodes[0x056] = STEINS_KEY_WORLD_2;

		keycodes[0x00E] = STEINS_KEY_BACKSPACE;
		keycodes[0x153] = STEINS_KEY_DELETE;
		keycodes[0x14F] = STEINS_KEY_END;
		keycodes[0x01C] = STEINS_KEY_ENTER;
		keycodes[0x001] = STEINS_KEY_ESCAPE;
		keycodes[0x147] = STEINS_KEY_HOME;
		keycodes[0x152] = STEINS_KEY_INSERT;
		keycodes[0x15D] = STEINS_KEY_MENU;
		keycodes[0x151] = STEINS_KEY_PAGE_DOWN;
		keycodes[0x149] = STEINS_KEY_PAGE_UP;
		keycodes[0x045] = STEINS_KEY_PAUSE;
		keycodes[0x039] = STEINS_KEY_SPACE;
		keycodes[0x00F] = STEINS_KEY_TAB;
		keycodes[0x03A] = STEINS_KEY_CAPS_LOCK;
		keycodes[0x145] = STEINS_KEY_NUM_LOCK;
		keycodes[0x046] = STEINS_KEY_SCROLL_LOCK;
		keycodes[0x03B] = STEINS_KEY_F1;
		keycodes[0x03C] = STEINS_KEY_F2;
		keycodes[0x03D] = STEINS_KEY_F3;
		keycodes[0x03E] = STEINS_KEY_F4;
		keycodes[0x03F] = STEINS_KEY_F5;
		keycodes[0x040] = STEINS_KEY_F6;
		keycodes[0x041] = STEINS_KEY_F7;
		keycodes[0x042] = STEINS_KEY_F8;
		keycodes[0x043] = STEINS_KEY_F9;
		keycodes[0x044] = STEINS_KEY_F10;
		keycodes[0x057] = STEINS_KEY_F11;
		keycodes[0x058] = STEINS_KEY_F12;
		keycodes[0x064] = STEINS_KEY_F13;
		keycodes[0x065] = STEINS_KEY_F14;
		keycodes[0x066] = STEINS_KEY_F15;
		keycodes[0x067] = STEINS_KEY_F16;
		keycodes[0x068] = STEINS_KEY_F17;
		keycodes[0x069] = STEINS_KEY_F18;
		keycodes[0x06A] = STEINS_KEY_F19;
		keycodes[0x06B] = STEINS_KEY_F20;
		keycodes[0x06C] = STEINS_KEY_F21;
		keycodes[0x06D] = STEINS_KEY_F22;
		keycodes[0x06E] = STEINS_KEY_F23;
		keycodes[0x076] = STEINS_KEY_F24;
		keycodes[0x038] = STEINS_KEY_LEFT_ALT;
		keycodes[0x01D] = STEINS_KEY_LEFT_CONTROL;
		keycodes[0x02A] = STEINS_KEY_LEFT_SHIFT;
		keycodes[0x15B] = STEINS_KEY_LEFT_SUPER;
		keycodes[0x137] = STEINS_KEY_PRINT_SCREEN;
		keycodes[0x138] = STEINS_KEY_RIGHT_ALT;
		keycodes[0x11D] = STEINS_KEY_RIGHT_CONTROL;
		keycodes[0x036] = STEINS_KEY_RIGHT_SHIFT;
		keycodes[0x15C] = STEINS_KEY_RIGHT_SUPER;
		keycodes[0x150] = STEINS_KEY_DOWN;
		keycodes[0x14B] = STEINS_KEY_LEFT;
		keycodes[0x14D] = STEINS_KEY_RIGHT;
		keycodes[0x148] = STEINS_KEY_UP;

		keycodes[0x052] = STEINS_KEY_KP_0;
		keycodes[0x04F] = STEINS_KEY_KP_1;
		keycodes[0x050] = STEINS_KEY_KP_2;
		keycodes[0x051] = STEINS_KEY_KP_3;
		keycodes[0x04B] = STEINS_KEY_KP_4;
		keycodes[0x04C] = STEINS_KEY_KP_5;
		keycodes[0x04D] = STEINS_KEY_KP_6;
		keycodes[0x047] = STEINS_KEY_KP_7;
		keycodes[0x048] = STEINS_KEY_KP_8;
		keycodes[0x049] = STEINS_KEY_KP_9;
		keycodes[0x04E] = STEINS_KEY_KP_ADD;
		keycodes[0x053] = STEINS_KEY_KP_DECIMAL;
		keycodes[0x135] = STEINS_KEY_KP_DIVIDE;
		keycodes[0x11C] = STEINS_KEY_KP_ENTER;
		keycodes[0x059] = STEINS_KEY_KP_EQUAL;
		keycodes[0x037] = STEINS_KEY_KP_MULTIPLY;
		keycodes[0x04A] = STEINS_KEY_KP_SUBTRACT;

		for (scancode = 0; scancode < 512; scancode++)
		{
			if (keycodes[scancode] > 0)
				scancodes[keycodes[scancode]] = scancode;
		}
	}

	LRESULT WindowsWindow::MsgProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
	{
		//if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		//	return true;
		WindowData* windowDataPtr = static_cast<WindowData*>(GetUserData());

		switch (_msg)
		{
		case WM_SIZE:
		{
			const int width = LOWORD(_lParam);
			const int height = HIWORD(_lParam);

			if (windowDataPtr != nullptr)
			{
				windowDataPtr->width = width;
				windowDataPtr->height = height;

				WindowResizeEvent event(width, height);
				if (windowDataPtr->eventCallbackFn != nullptr)
				{
					windowDataPtr->eventCallbackFn(event);
				}
			}
			break;
		}
		{
			KeyPressedEvent event(static_cast<int>(_wParam), 0);
			if (windowDataPtr->eventCallbackFn != nullptr)
			{
				windowDataPtr->eventCallbackFn(event);
			}
			break;
		}
		{
			KeyReleasedEvent event(static_cast<int>(_wParam));
			windowDataPtr->eventCallbackFn(event);
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			int key, scancode;
			const int action = (HIWORD(_lParam) & KF_UP) ? STEINS_RELEASE : STEINS_PRESS;

			scancode = (HIWORD(_lParam) & (KF_EXTENDED | 0xff));
			if (!scancode)
			{
				scancode = MapVirtualKeyW((UINT)_wParam, MAPVK_VK_TO_VSC);
			}
			// HACK: Alt+PrtSc has a different scancode than just PrtSc
			if (scancode == 0x54)
				scancode = 0x137;

			// HACK: Ctrl+Pause has a different scancode than just Pause
			if (scancode == 0x146)
				scancode = 0x45;

			// HACK: CJK IME sets the extended bit for right Shift
			if (scancode == 0x136)
				scancode = 0x36;

			key = keycodes[scancode];

			// The Ctrl keys require special handling
			if (key == VK_CONTROL)
			{
				if (HIWORD(_lParam) & KF_EXTENDED)
					key = VK_RCONTROL;
				else
					key = VK_LCONTROL;
			}
			else if (_wParam == VK_PROCESSKEY)
			{
				// IME notifies that keys have been filtered by setting the
				// virtual key-code to VK_PROCESSKEY
				break;
			}

			WindowInputKey(key, action);

			break;
		}

		case WM_SYSCOMMAND:
			if ((_wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_MOUSEMOVE:
		{
			const int x = GET_X_LPARAM(_lParam);
			const int y = GET_Y_LPARAM(_lParam);

			WindowCursorPos(x, y);

			return 0;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			int i, button, action;

			if (_msg == WM_LBUTTONDOWN || _msg == WM_LBUTTONUP)
			{
				button = STEINS_MOUSE_BUTTON_LEFT;
			}
			else if (_msg == WM_RBUTTONDOWN || _msg == WM_RBUTTONUP)
			{
				button = STEINS_MOUSE_BUTTON_RIGHT;
			}
			else if (_msg == WM_MBUTTONDOWN || _msg == WM_MBUTTONUP)
			{
				button = STEINS_MOUSE_BUTTON_MIDDLE;
			}
			else if (GET_XBUTTON_WPARAM(_wParam) == XBUTTON1)
			{
				button = STEINS_MOUSE_BUTTON_4;
			}
			else
			{
				button = STEINS_MOUSE_BUTTON_5;
			}

			if (_msg == WM_LBUTTONDOWN || _msg == WM_RBUTTONDOWN ||
				_msg == WM_MBUTTONDOWN || _msg == WM_XBUTTONDOWN)
			{
				action = STEINS_PRESS;
			}
			else
			{
				action = STEINS_RELEASE;
			}

			for (i = 0; i <= STEINS_MOUSE_BUTTON_LAST; i++)
			{
				if (mouseStates[i] == STEINS_PRESS)
					break;
			}

			if (i > STEINS_MOUSE_BUTTON_LAST)
				SetCapture(_hwnd);

			WindowMouseClick(button, action);

			for (i = 0; i <= STEINS_MOUSE_BUTTON_LAST; i++)
			{
				if (mouseStates[i] == STEINS_PRESS)
					break;
			}

			if (i > STEINS_MOUSE_BUTTON_LAST)
				ReleaseCapture();

			if (_msg == WM_XBUTTONDOWN || _msg == WM_XBUTTONUP)
				return true;

			break;
		}

		case WM_MOUSEWHEEL:
		{
			WindowMouseScroll(0.0, (SHORT)HIWORD(_wParam) / (double)WHEEL_DELTA);
			return 0;
		}

		case WM_MOUSEHWHEEL:
		{
			// This message is only sent on Windows Vista and later
			// NOTE: The X-axis is inverted for consistency with macOS and X11
			WindowMouseScroll(-((SHORT)HIWORD(_wParam) / (double)WHEEL_DELTA), 0.0);
			return 0;
		}


		case WM_CLOSE:
		{
			if (windowDataPtr != nullptr)
			{
				WindowCloseEvent event;
				if (windowDataPtr->eventCallbackFn != nullptr)
				{
					windowDataPtr->eventCallbackFn(event);
				}
			}
			return 0;
		}
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}
		return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
	}
}
