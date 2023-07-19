#pragma once


#include <Windows.h>
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <algorithm>
#include <iostream>

namespace instance_check
{

	/*			This is for the check if a instance is already running		*/ 
	static  HANDLE hMutex;
	static constexpr  const char* mutex_name = "Image_Converter_Mutex";


	bool is_already_running();
	void handle_multiple_instances();


}

