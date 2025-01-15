
#include "main.h"
#include "application.h"

using namespace Application;

/// <summary>
/// main func in windows
/// </summary>
int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	Manager& app_manager = Manager::Instance();

	if (app_manager.Initialize()) return -1;
	app_manager.Run();
	app_manager.Terminate();

	return 0;
}
