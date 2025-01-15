
#include "main.h"
#include "application.h"
#include "window.h"
#include "directx11_wrapper.h"

namespace Application
{
	/// <summary>
	/// instantiate with the Singleton Method Design Pattern
	/// </summary>
	Manager& Manager::Instance()
	{
		static Manager s_instance;
		return s_instance;
	}

	/// <summary>
	/// initialization process for apps
	/// </summary>
	int Manager::Initialize()
	{
		Window::Manager::Instance().Initialize();
		DirectXWrapper::Manager::Instance().Initialize();

		return 0;
	}

	/// <summary>
	/// termination process for apps
	/// </summary>
	void Manager::Terminate()
	{
		DirectXWrapper::Manager::Instance().Terminate();
		Window::Manager::Instance().Terminate();
	}

	/// <summary>
	/// running process for apps
	/// </summary>
	void Manager::Run()
	{
		Window::Manager window_manager = Window::Manager::Instance();
		
		HWND& hWnd = window_manager.GetWindowHandle();
		MSG message;
		
		// show (display) and update window
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		// busy loop
		while (1)
		{
			// if there are messages in the Windows Message Queue, process the following
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				// if "PostQuitMessage(0)" is called in the Window Procedure, exit the busy loop
				if (message.message == WM_QUIT) break;

				// translate and dispatch of messages
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				window_manager.Run();
			}
		}
	}
}
