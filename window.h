
#pragma once

#ifdef _DEBUG
// the flag for window switchable
#define WINDOW_MODE_SWITCHABLE_ENABLED
#endif

namespace Window
{
	//--------------------------------------------------------
	// constant
	//--------------------------------------------------------
	// window name
	constexpr LPCTSTR WINDOW_NAME       = "AppWindow";
	constexpr LPCTSTR WINDOW_CLASS_NAME = "AppWindowClass";
	constexpr LPCTSTR WINDOW_MENU_NAME  = "AppWindowMenu";

	// window size
	constexpr int WINDOW_SIZE_WIDTH  = 960;
	constexpr int WINDOW_SIZE_HEIGHT = 540;

	//--------------------------------------------------------
	// enumerator
	//--------------------------------------------------------
#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
	/// <summary>
	/// enumeration of display modes
	/// </summary>
	enum class DisplayMode
	{
		Windowed,
		Fullscreen,

		Maximum
	};
#endif

	//--------------------------------------------------------
	// manager class
	//--------------------------------------------------------
	class Manager
	{
		WNDCLASSEX _wcex;
		HWND _hWnd;

		BOOL _isWindowedMode;

		// time
		DWORD _preExecTime;
		DWORD _preFpsTime;
		DWORD _curTime;
		DWORD _frameCount;

#ifdef _DEBUG
		int _fpsCount;
		TCHAR _debugStr[2048];
#endif

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
		HMENU _hMenu;
		HMENU _hPopupMenu;
		DisplayMode _displayMode;
		DEVMODE _deviceMode;
#endif

		//-----------------------------------
		// private funcs
		//-----------------------------------
		void CreateWindowClass();
		void CreateWindowHandle();
		void RegisterWindowClass();

#ifdef _DEBUG
		void DisplayWindowMessageBox();
#endif

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
		void CreateFullscreenSettings();
		void CreateMenus();
		void UpdateMenus();
#endif

		//-----------------------------------
		// public funcs
		//-----------------------------------
	public:
		Manager();
		static Manager& Instance();

		HRESULT Initialize();
		void Terminate();
		void Run();

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
		void ChangeWindowModeWindowed();
		void ChangeWindowModeFullscreen();
#endif

		//-----------------------------------
		// getter funcs
		//-----------------------------------
		HINSTANCE& GetInstanceHandle();
		HWND& GetWindowHandle();
		BOOL GetIsWindowedMode();
	};
}
