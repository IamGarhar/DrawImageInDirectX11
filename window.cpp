
#include <tchar.h>
#include "main.h"
#include "window.h"
#include "directx11_wrapper.h"

namespace Window
{
	/// <summary>
	/// constructor for window
	/// </summary>
	Manager::Manager()
	{
		_wcex = {};
		_hWnd = {};

		_isWindowedMode = TRUE;

		_preExecTime = 0;
		_preFpsTime  = 0;
		_curTime     = 0;
		_frameCount  = 0;

#ifdef _DEBUG
		_fpsCount = 0;
		ZeroMemory(_debugStr, sizeof(_debugStr));
#endif

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
		_hMenu       = nullptr;
		_hPopupMenu  = nullptr;
		_displayMode = DisplayMode::Windowed;
		_deviceMode  = {};
#endif
	}
	
	/// <summary>
	/// instantiate with the Singleton Method Design Pattern
	/// </summary>
	Manager& Manager::Instance()
	{
		static Manager s_instance;
		return s_instance;
	}

	/// <summary>
	/// initialization process for window
	/// </summary>
	HRESULT Manager::Initialize()
	{
		// settings of window class
		CreateWindowClass();
		RegisterWindowClass();

		// create window
		CreateWindowHandle();

#ifdef WINDOW_MODE_SWITCHABLE_ENABLED
		CreateFullscreenSettings();
		CreateMenus();
		UpdateMenus();
#endif

#ifdef _DEBUG
		DisplayWindowMessageBox();
#endif

		// requests a min resolution for periodic timers
		timeBeginPeriod(1);

		// initialize time counters
		_preExecTime = _preFpsTime = timeGetTime();
		_curTime = _frameCount = 0;

		return 0;
	}

	/// <summary>
	/// termination process for window
	/// </summary>
	void Manager::Terminate()
	{
		// clears a min resolution for periodic timers
		timeEndPeriod(1);

		UnregisterClass(WINDOW_CLASS_NAME, _wcex.hInstance);
	}

	/// <summary>
	/// running process for window
	/// </summary>
	void Manager::Run()
	{
		// get the current system time
		_curTime = timeGetTime();

		// initialize frame count every second
		if ((_curTime - _preFpsTime) >= 1000)
		{
#ifdef _DEBUG
			_fpsCount = _frameCount;
#endif

			_preFpsTime = _curTime;
			_frameCount = 0;
		}

		// process at 120 fps
		if ((_curTime - _preExecTime) >= (1000 / 120))
		{
			_preExecTime = _curTime;

#ifdef _DEBUG
			// set debug strings
			wsprintf(_debugStr, WINDOW_NAME);
			wsprintf(&_debugStr[strlen(_debugStr)], _T(" - fps [ %d ]"), _fpsCount);
#endif

			// if you run a graphics pipeline, do it here
			DirectXWrapper::Manager::Instance().Update();
			DirectXWrapper::Manager::Instance().Draw();

#ifdef _DEBUG
			// display and clear debug strings
			SetWindowText(_hWnd, _debugStr);
			ZeroMemory(_debugStr, sizeof(_debugStr));
#endif

			_frameCount++;
		}
	}
}
