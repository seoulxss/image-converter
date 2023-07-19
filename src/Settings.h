#pragma once


#include <array>
#include <string>

namespace settings
{
	static bool b_window_done = false;

	static unsigned short int tab = 0;
	static bool s_beep = false;
	static bool s_folder_open = false;

	static int s_quality = 85;
	static bool s_remove_meta_data = false;

	static int s_depth = 2;


	static const char* current_compression = "JPEG-Compression";

	static std::array<std::string, 2> list_of_formats
	{
		".png", ".jpg"
	};


	static std::array<const char*, 3> list_of_compressions_jpg
	{
		"JPEG-Compression", "Lossless-JPEGCompression", "JPEG2000-Compression"
	};

	static std::array<std::string, 2> list_of_compressions_png
	{
		"ZipCompression", "RLECompression"
	};


}