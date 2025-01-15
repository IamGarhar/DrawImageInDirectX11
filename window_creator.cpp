
#include "main.h"
#include "window.h"

namespace Window
{
	/// <summary>
	/// window procedure
	/// </summary>
	LRESULT CALLBACK WindowProcedure(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (msg)
		{
			// if Esc key is pressed, post "WM_DESTROY" to the Windows Message Queue (not MSMQ)
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
			break;

			// if "WM_DESTROY" is received from the Windows Message Queue, post "WM_QUIT" there
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
			// processes when the user selects a command from the window menu
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case static_cast<int>(DisplayMode::Windowed):
				Window::Manager::Instance().ChangeWindowModeWindowed();
				break;

			case static_cast<int>(DisplayMode::Fullscreen):
				Window::Manager::Instance().ChangeWindowModeFullscreen();
				break;
			}
			break;
#endif

		default:
			// calls the default window procedure
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

	/// <summary>
	/// create a window class
	/// </summary>
	void Manager::CreateWindowClass()
	{
		this->_wcex =
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			WindowProcedure,
			0,
			0,
			_wcex.hInstance,
			LoadIcon(_wcex.hInstance, IDI_WINLOGO),
			LoadCursor(_wcex.hInstance, IDC_ARROW),
			reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
			WINDOW_MENU_NAME,
			WINDOW_CLASS_NAME,
			LoadIcon(_wcex.hInstance, IDI_WINLOGO)
		};
	}

	/// <summary>
	/// create a window handle
	/// </summary>
	void Manager::CreateWindowHandle()
	{
		this->_hWnd = CreateWindow
		(
			WINDOW_CLASS_NAME,
			WINDOW_NAME,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WINDOW_SIZE_WIDTH  + GetSystemMetrics(SM_CXDLGFRAME) * 2,
			WINDOW_SIZE_HEIGHT + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
			nullptr,
			nullptr,
			_wcex.hInstance,
			nullptr
		);
	}

	/// <summary>
	/// registration of window class
	/// </summary>
	void Manager::RegisterWindowClass()
	{
		RegisterClassEx(&_wcex);
	}

#ifdef _DEBUG
	/// <summary>
	/// display a message box at window startup
	/// </summary>
	void Manager::DisplayWindowMessageBox()
	{
		switch (MessageBoxEx(nullptr, "Do you wanna play in windowed mode?", "START-UP MODE SELECT",
			MB_YESNOCANCEL | MB_ICONQUESTION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)))
		{
		case IDYES:
			_isWindowedMode = TRUE;
			break;

		case IDNO:
			_isWindowedMode = FALSE;
			break;

		case IDCANCEL:
		default:
			DestroyWindow(_hWnd);
			break;
		}
	}
#endif

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
	/// <summary>
	/// create full-screen settings
	/// </summary>
	void Manager::CreateFullscreenSettings()
	{
		_deviceMode.dmSize = sizeof(_deviceMode);
		_deviceMode.dmPelsWidth  = GetSystemMetrics(SM_CXSCREEN);
		_deviceMode.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
		_deviceMode.dmBitsPerPel = 32;
		_deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	}

	/// <summary>
	/// create window menus and popup menus
	/// </summary>
	void Manager::CreateMenus()
	{
		// popup menu
		_hPopupMenu = CreatePopupMenu();
		AppendMenu(_hPopupMenu, MF_STRING, static_cast<int>(DisplayMode::Windowed), "&Windowed mode");
		AppendMenu(_hPopupMenu, MF_SEPARATOR, 0, nullptr);
		AppendMenu(_hPopupMenu, MF_STRING, static_cast<int>(DisplayMode::Fullscreen), "&Fullscreen mode");

		// window menu
		_hMenu = CreateMenu();
		AppendMenu(_hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(_hPopupMenu), "&Display settings");
		SetMenu(_hWnd, _hMenu);
	}

	/// <summary>
	/// update window menus and popup menus
	/// </summary>
	void Manager::UpdateMenus()
	{
		EnableMenuItem(_hPopupMenu, static_cast<int>(DisplayMode::Windowed),   (_displayMode == DisplayMode::Windowed)   ? MF_GRAYED : MF_ENABLED);
		EnableMenuItem(_hPopupMenu, static_cast<int>(DisplayMode::Fullscreen), (_displayMode == DisplayMode::Fullscreen) ? MF_GRAYED : MF_ENABLED);
	}

	/// <summary>
	/// change window mode to windowed mode
	/// </summary>
	void Manager::ChangeWindowModeWindowed()
	{
		// change an attribute of the spacified window
		SetWindowLong(_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		// set the window position
		SetWindowPos(_hWnd, HWND_TOP, 0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

		ShowWindow(_hWnd, SW_NORMAL);

		_displayMode = DisplayMode::Windowed;
		UpdateMenus();
	}

	/// <summary>
	/// change window mode to full-screen mode
	/// </summary>
	void Manager::ChangeWindowModeFullscreen()
	{
		if (ChangeDisplaySettings(&_deviceMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(nullptr, "Failed to switch to full-screen mode", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		// change an attribute of the spacified window
		SetWindowLong(_hWnd, GWL_STYLE, WS_POPUP);

		// set the window position
		SetWindowPos(_hWnd, HWND_TOP, 0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

		ShowWindow(_hWnd, SW_MAXIMIZE);

		_displayMode = DisplayMode::Fullscreen;
		UpdateMenus();
	}
#endif
}
