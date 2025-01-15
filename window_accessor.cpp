
#include "main.h"
#include "window.h"

namespace Window
{
	/// <summary>
	/// getter func for instance handle
	/// </summary>
	HINSTANCE& Manager::GetInstanceHandle()
	{
		return _wcex.hInstance;
	}

	/// <summary>
	/// getter func for window handle
	/// </summary>
	HWND& Manager::GetWindowHandle()
	{
		return _hWnd;
	}

	/// <summary>
	/// getter func for windowed mode flag
	/// </summary>
	BOOL Manager::GetIsWindowedMode()
	{
		return _isWindowedMode;
	}
}
