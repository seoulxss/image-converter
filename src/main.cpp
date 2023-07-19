#include <iostream>
#include <memory>
#include "Backend/Instance/processes.h"
#include "Window/Window.h"


//Entry point of the windows program app
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//Check if program is already running
	instance_check::handle_multiple_instances();

#ifdef NDEBUG
	//Release mode
	const std::unique_ptr<CWindow> Window (new CWindow(500, 300, ::app_name_release));
	Window->Run();

#else
	//Debug mode
	const std::unique_ptr<CWindow> Window (new CWindow(500, 300, ::app_name_debug));
	Window->Run();

#endif


}
