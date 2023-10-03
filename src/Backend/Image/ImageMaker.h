#pragma once
#include <filesystem>
#include "Magick++.h"
#include <math.h>
#include <Windows.h>

#ifdef NDEBUG

#pragma comment(lib, "CORE_RL_Magick++_.lib")
#pragma comment(lib, "CORE_RL_MagickCore_.lib")
#pragma comment(lib, "CORE_RL_MagickWand_.lib")

#else

#pragma comment(lib, "CORE_DB_Magick++_.lib")
#pragma comment(lib, "CORE_DB_MagickCore_.lib")
#pragma comment(lib, "CORE_DB_MagickWand_.lib")

#endif
#include "../../../ext/ImGui/imgui.h"


#include "..\..\Settings.h"


// This is just horrible, don't look at it xD.
class ImageMaker
{

public:

	static void init_Image_Maker();

	[[nodiscard]] static bool Convert_images(const int quality, const bool dele_metadata, const int depth, const char* compression) noexcept;

	static std::vector<std::string> get_all_Images();
	static bool create_output_dir();

	static std::vector<std::string> get_files() noexcept;
	static std::vector<std::string> set_files(const std::vector<std::string>& set_files) noexcept;

	static std::string get_file_path() noexcept;
	static std::string set_file_path(const std::string& set_file_path) noexcept;

	static unsigned int get_current_state() noexcept;
	static unsigned int set_current_state(const unsigned int& new_cur_state) noexcept;

	static unsigned int get_max_state() noexcept;
	static unsigned int set_max_state(const unsigned& new_max_state) noexcept;

	static std::vector<std::string> set_all_image_string_vec(const std::vector<std::string>& new_string_vec) noexcept;
	static std::vector<std::string> get_all_image_string_vec() noexcept;
	static std::vector<std::string> get_image_names_only(const std::vector<std::string>& string_vec);

	static std::string set_output_dir(const std::string& new_dir);
	static std::string get_output_dir();

	static float get_curr_image_counter() noexcept;
	static float set_curr_image_counter(const float& new_float_val) noexcept;

	static float get_curr_image_counter_max() noexcept;
	static float set_curr_image_counter_max(const float& new_float_val) noexcept;
	static void reset_curr_image_all() noexcept;

private:
	
	static std::vector<std::string> m_image_files;
	static std::string m_file_path;
	static std::string m_output_dir;

	static std::string m_all_images_string;
	static std::vector<std::string> m_vec_all_images_string;

	static unsigned int m_cur_state;
	static unsigned int m_max_state;

	static float m_image_counter;
	static float m_image_counter_max;

};
