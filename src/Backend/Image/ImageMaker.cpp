#include "ImageMaker.h"



std::string ImageMaker::m_file_path;
std::vector<std::string> ImageMaker::m_image_files = {};

unsigned int ImageMaker::m_cur_state = 0;
unsigned int ImageMaker::m_max_state = 0;

std::string ImageMaker::m_output_dir;

std::string ImageMaker::m_all_images_string;
std::vector<std::string> ImageMaker::m_vec_all_images_string = {};

float ImageMaker::m_image_counter = 0;
float ImageMaker::m_image_counter_max = 0;

void ImageMaker::init_Image_Maker()
{
	Magick::InitializeMagick(nullptr);
	m_image_files.reserve(100);

}

bool ImageMaker::Convert_images(const int quality, const bool dele_metadata, const int depth, const char* compression) noexcept
{
	const  std::string output_name = "output_";
	int i = 0;

	if (ImageMaker::get_files().empty())
		return false;

	if (ImageMaker::get_file_path().empty())
		return false;

	ImageMaker::set_curr_image_counter_max(static_cast<float>(ImageMaker::get_files().size()));

	try
	{
		for (unsigned int i = 0; i < m_image_files.size(); i++)
		{
			const auto colors = static_cast<size_t>(std::pow(2, depth));


			Magick::Image Image;
			Image.read(m_image_files[i]);


			if (dele_metadata)
			{
				Image.strip();
			}


			Image.magick("JPEG");
			Image.depth(static_cast<size_t>(depth));
			Image.quantizeColors(colors);
			Image.quality(static_cast<size_t>(quality));


			if (compression == "JPEG-Compression")
			{
				Image.compressType(MagickCore::JPEGCompression);
			}

			else if (compression == "Lossless-JPEGCompression")
			{
				Image.compressType(MagickCore::LosslessJPEGCompression);
			}

			else if (compression == "JPEG2000-Compression")
			{
				Image.compressType(MagickCore::JPEG2000Compression);
			}

			Image.write(m_output_dir + "\\" + output_name + std::to_string(i) + ".jpg");

			ImageMaker::set_curr_image_counter(ImageMaker::get_curr_image_counter() + 1);
		}

	}

	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Error", MB_ICONERROR);
		return false;
	}






	return true;
}

std::vector<std::string> ImageMaker::get_all_Images()
{

	std::vector<std::string> files;
	files.reserve(50);

	if (m_file_path.length() == 0)
		return { "" };


	for (const auto & entry : std::filesystem::directory_iterator(m_file_path))
	{

		const std::filesystem::path& path = entry;

		std::string end_string = path.string();

		const auto string_check = end_string.find(".png");


		if (string_check != std::string::npos)
		{
			files.push_back(end_string);

		}

		else
			continue;

	}

	return {files};
}

std::vector<std::string> ImageMaker::get_files() noexcept
{
	return ImageMaker::m_image_files;
}

std::string ImageMaker::get_file_path() noexcept
{
	return ImageMaker::m_file_path;
}

std::vector<std::string> ImageMaker::set_files(const std::vector<std::string>& set_files) noexcept
{
	return ImageMaker::m_image_files = set_files;
}

std::string ImageMaker::set_file_path(const std::string& set_file_path) noexcept
{
	return ImageMaker::m_file_path = set_file_path;
}

unsigned ImageMaker::get_current_state() noexcept
{
	return ImageMaker::m_cur_state;
}

unsigned ImageMaker::set_current_state(const unsigned& new_cur_state) noexcept
{
	return ImageMaker::m_cur_state = new_cur_state;
}

unsigned ImageMaker::get_max_state() noexcept
{
	return ImageMaker::m_max_state;
}

unsigned ImageMaker::set_max_state(const unsigned& new_max_state) noexcept
{
	return ImageMaker::m_max_state = new_max_state;
}

std::vector<std::string> ImageMaker::set_all_image_string_vec(const std::vector<std::string>& new_string_vec) noexcept
{
	return ImageMaker::m_vec_all_images_string = new_string_vec;
}

std::vector<std::string> ImageMaker::get_all_image_string_vec() noexcept
{
	return ImageMaker::m_vec_all_images_string;
}

std::vector<std::string> ImageMaker::get_image_names_only(const std::vector<std::string>& string_vec)
{
    if (get_file_path().empty())
        return {"0"};

    const auto file_path = get_file_path();
    std::vector<std::string> updated_vec; 

    for (const auto& str : string_vec)
    {
        std::string updated_str = str;
        const size_t pos = updated_str.find(get_file_path());

        if (pos != std::string::npos)
        {
            updated_str.erase(pos, file_path.length());
			updated_str.erase(0, 1);

        }

        updated_vec.push_back(updated_str); 
    }

    return updated_vec;
}

std::string ImageMaker::set_output_dir(const std::string& new_dir)
{
	return m_output_dir = new_dir;
}

std::string ImageMaker::get_output_dir()
{
	return m_output_dir;
}

float ImageMaker::get_curr_image_counter() noexcept
{
	return m_image_counter;
}

float ImageMaker::set_curr_image_counter(const float& new_float_val) noexcept
{
	return m_image_counter = new_float_val;
}

float ImageMaker::get_curr_image_counter_max() noexcept
{
	return m_image_counter_max;
}

float ImageMaker::set_curr_image_counter_max(const float& new_float_val) noexcept
{
	return m_image_counter_max = new_float_val;
}

void ImageMaker::reset_curr_image_all() noexcept
{
	m_image_counter = 0;
	m_image_counter_max = 0;
}

bool ImageMaker::create_output_dir()
{
	if (get_file_path().empty())
		return false;

	 
	std::string output_string_check = m_file_path + "\\" + "Output";


	do 
	{
		output_string_check.append("_1");
	} while (std::filesystem::exists(output_string_check));


	if (std::filesystem::create_directory(output_string_check))
	{
		set_output_dir(output_string_check);
		return true;
	}
		


	return false;

}
