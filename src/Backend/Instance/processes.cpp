#include "processes.h"




bool instance_check::is_already_running()
{

	hMutex = CreateMutexA(nullptr, TRUE, mutex_name);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return true;
	}

	return false;
}

void instance_check::handle_multiple_instances()
{
	if (instance_check::is_already_running() == true)
	{
		MessageBeep(MB_ICONERROR);
		MessageBoxA(nullptr, "There is already a instance of 'Image Converter' running!", "Error!", MB_ICONWARNING);
		exit(0);
	}

}

