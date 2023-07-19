#include "Window.h"
#include <stdio.h>
#include "../../resource.h"

#define WND_EXCEPT(hResult) CWindow::Exception(__LINE__, __FILE__, hResult)
#define WND_LAST_EXCEPT() CWindow::Exception(__LINE__, __FILE__, GetLastError())

CWindow::CWindowClass CWindow::CWindowClass::wnd_class_;
WNDCLASSEX CWindow::CWindowClass::m_window_class = {0};

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool ButtonCenteredOnLine(const char* label, float alignment)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}

void AlignForWidth(float width, float alignment)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}


//Constructor of C_UI, which initializes ImGui
CDirectX::C_UI::C_UI(const HWND& hWnd, const CDirectX& dxInstance) noexcept
{
	IMGUI_CHECKVERSION();
	pImGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	//Disable ImGui ini
	io.IniFilename = nullptr;

	// Setup Dear ImGui style
    ImGui::StyleColorsDark();
	set_style();
	CDirectX::C_UI::init_fonts();
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(dxInstance.pDevice.Get(), dxInstance.pDeviceContext.Get());


	

}

//Destructor of C_UI, which shutdowns ImGui
CDirectX::C_UI::~C_UI()
{
	//Releasing pointers of the fonts to prevent crash...
	pNew_Absolute_empire_font_Headers.release();
	pNew_Athletic_font_normal.release();
	pNew_Athletic_font_normal_small.release();
	pNew_Roboto_font.release();

	//Shutdown ImGui
	ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

//This renders the ImGui Menu
void CDirectX::C_UI::Render(CDirectX& DirectX_Cui_instance)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (settings::b_window_done == false)
	{
		ImageMaker::init_Image_Maker();
		CDirectX::C_UI::set_WindowSize();
	}
		
		
		

    ImGui::Begin("Hi", nullptr, ImGui_Flags);
	{

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnOffset(1, 200 - 20);

		{
			ImGui::PushFont(font_else);

			// ImGui::PushFont(icon_font);
			if (ImGui::Button("Settings", ImVec2(200 - 50, 42)))
				settings::tab = 1;

			// ImGui::PopFont();


			if (ImGui::Button("List", ImVec2(200 - 50, 42)))
				settings::tab = 2;


			if (ImGui::Button("Convert", ImVec2(200 - 50, 42)))
				settings::tab = 3;

			ImGui::PopFont();
		}

		ImGui::NextColumn();

		{
			if (settings::tab == 1)
			{
				ImGui::PushFont(font_else);
				{
					ImGui::PushFont(font_headers);
					const ImVec2 textSize = ImGui::CalcTextSize("Settings");
					const ImVec2 windowSize = ImGui::GetWindowSize();
					ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.72f);
					ImGui::Text("Settings");
					ImGui::PopFont();
					ImGui::Separator();
					ImGui::Spacing();
				}
				

				{
					ImGui::Checkbox("Beep when finish", &settings::s_beep);
				}

				{
					ImGui::Checkbox("Open folder when finish", &settings::s_folder_open);
				}
				
				{
					ImGui::Checkbox("Remove metadata", &settings::s_remove_meta_data); ImGui::SameLine();

					ImGui::PushFont(font_roboto);
					HelpMarker("Deletes image parameters");
					ImGui::PopFont();
					ImGui::Spacing();
				}

				{
					ImGui::SliderInt("Quality", &settings::s_quality, 1, 100);
				}

				{
					ImGui::SliderInt("depth", &settings::s_depth, 1, 32);
				}

				{
					const float windowWidth = ImGui::GetWindowContentRegionWidth();
					constexpr float selectableWidth = 100.0f;
					const float posX = (windowWidth - selectableWidth) * 0.490f;
					ImGui::SetCursorPosX(posX);

					const char* items[] =
					{
						settings::list_of_compressions_jpg[0],
						settings::list_of_compressions_jpg[1],
						settings::list_of_compressions_jpg[2]
					};

					static const char* current_compression = items[0];

					


					if (ImGui::BeginCombo("Method", current_compression)) // The second parameter is the label previewed before opening the combo.
					{
						static bool font_check = false;

						for (auto& item : items)
						{
							if (!font_check)
							{
								ImGui::PushFont(font_roboto);
								font_check = true;
							}

							const bool is_selected = (current_compression == item);
							if (ImGui::Selectable(item, is_selected))
							{
								current_compression = item;
								settings::current_compression = current_compression;
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
						}
						font_check = false;
						ImGui::PopFont();

						ImGui::EndCombo();
					}
					 

					ImGui::Spacing();
				}




				ImGui::PopFont();
			}

			else if (settings::tab == 2)
			{

				{
					ImGui::PushFont(font_headers);
					const ImVec2 textSize = ImGui::CalcTextSize("List of images");
					const ImVec2 windowSize = ImGui::GetWindowSize();
					ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.79f);
					ImGui::Text("List of images");
					ImGui::PopFont();
					ImGui::Separator();
					ImGui::Spacing();
				}

				{
					ImGui::PushFont(font_else_small);

					ImageMaker::set_all_image_string_vec(ImageMaker::get_image_names_only(ImageMaker::get_all_Images()));
					const auto imageVec = ImageMaker::get_all_image_string_vec();
					for (const auto& i : imageVec)
					{
						ImGui::Text(i.c_str());
					}

					ImGui::PopFont();
				}

				

			}

			else if (settings::tab == 3)
			{

				{
					ImGui::PushFont(font_headers);
					const ImVec2 textSize = ImGui::CalcTextSize("CONVERT");
					const ImVec2 windowSize = ImGui::GetWindowSize();
					ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.72f);
					ImGui::Text("CONVERT");
					ImGui::PopFont();
					ImGui::Separator();
					ImGui::Spacing();
				}

				ImGui::PushFont(font_else);

				if (ImGui::Button("Path", ImVec2(200 - 60, 40)))
				{
					ImageMaker::set_file_path(Convert_PWSTR_To_String(C_UI::open_file_dialog()));
					ImGui::SameLine();
				}

				ImGui::SameLine();


				//ImGui::SameLine();
				if (ImGui::Button("Convert!", ImVec2(200 - 60, 40)))
				{


					if (ImageMaker::create_output_dir())
					{
						ImageMaker::set_files(ImageMaker::get_all_Images());


						if (ImageMaker::Convert_images(settings::s_quality,
							settings::s_remove_meta_data,
							settings::s_depth, settings::current_compression))
						{

							if (settings::s_beep)
							{
								MessageBeep(MB_OK);
							}

							if (settings::s_folder_open)
							{
								ShellExecute(nullptr, "open", ImageMaker::get_file_path().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
							}



						}





					}



				}




				ImGui::PopFont();
			}





		}






	}
    ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();

	constexpr float clearColor[] = { 0.5f, 0.3f, 0.8f, 1.0f }; 
	DirectX_Cui_instance.pDeviceContext->OMSetRenderTargets(1, DirectX_Cui_instance.pRenderTargetView.GetAddressOf(), nullptr);
	DirectX_Cui_instance.pDeviceContext->ClearRenderTargetView(DirectX_Cui_instance.pRenderTargetView.Get(),clearColor);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool CDirectX::C_UI::set_WindowSize()
{

	//Gets the Window Handle of the current application
#ifdef NDEBUG
		const auto hWnd = FindWindowA(nullptr, ::app_name_release);
#else
		const auto hWnd = FindWindowA(nullptr, ::app_name_debug);
#endif


	//Getting the client rect
	RECT rect;
	GetClientRect(hWnd, &rect);
	const int width = rect.right - rect.left;
	const int height = rect.bottom - rect.top;

	//Setting the SizeConstraints
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

	//Setting it to the right pos and size
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f)); 
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)));
	return true;
}

//sets the ImGui Style
void CDirectX::C_UI::set_style()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 0.75f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.98f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.98f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	// Anpassung der Rundungsecken
	//style->WindowRounding = 4.0f;
	style->FrameRounding = 4.0f;
	style->PopupRounding = 4.0f;
	style->ScrollbarRounding = 4.0f;
	style->GrabRounding = 4.0f;
	style->TabRounding = 4.0f;
	style->TabBorderSize = 0.0f;
	style->FrameBorderSize = 0.0f;
	style->WindowBorderSize = 0.0f;



}

bool CDirectX::C_UI::init_fonts()
{
	//copying the font into the memory
	pNew_Athletic_font_normal =  std::make_unique<unsigned char[]>(sizeof(new_Athletic_font_arr));
	std::memcpy(pNew_Athletic_font_normal.get(), new_Athletic_font_arr, sizeof(new_Athletic_font_arr));

	pNew_Absolute_empire_font_Headers = std::make_unique<unsigned char[]>(sizeof(aAbsoluteEmpire_arr));
	std::memcpy(pNew_Absolute_empire_font_Headers.get(), aAbsoluteEmpire_arr, sizeof(aAbsoluteEmpire_arr));

	pNew_Athletic_font_normal_small = std::make_unique<unsigned char []>(sizeof(new_Athletic_font_arr));
	std::memcpy(pNew_Athletic_font_normal_small.get(), new_Athletic_font_arr, sizeof(new_Athletic_font_arr));

	pNew_Roboto_font = std::make_unique<unsigned char[]>(sizeof(new_RobotoRegular_font_arr));
	std::memcpy(pNew_Roboto_font.get(), new_RobotoRegular_font_arr, sizeof(new_RobotoRegular_font_arr));

	//Initializing the fonts etc..
	const ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontDefault();


	//Font one
	const ImFontConfig new_Athletic_font_config;
	font_else = io.Fonts->AddFontFromMemoryTTF(
		pNew_Athletic_font_normal.get(),
		sizeof(new_Athletic_font_arr),
		22.0f,
		&new_Athletic_font_config,
		nullptr);

		const ImFontConfig new_Athletic_font_config_small;
	font_else_small = io.Fonts->AddFontFromMemoryTTF(
		pNew_Athletic_font_normal_small.get(),
		sizeof(new_Athletic_font_arr),
		14.0f,
		&new_Athletic_font_config_small,
		nullptr);


	//Font two
	const ImFontConfig new_Absolute_empire_font_headers_config;
	font_headers = io.Fonts->AddFontFromMemoryTTF(
		pNew_Absolute_empire_font_Headers.get(),
		sizeof(aAbsoluteEmpire_arr),
		28.0f,
		&new_Absolute_empire_font_headers_config,
		nullptr);


	//Font three
		const ImFontConfig new_roboto_font_config;
	font_roboto = io.Fonts->AddFontFromMemoryTTF(
		pNew_Roboto_font.get(),
		sizeof(new_RobotoRegular_font_arr),
		22.0f,
		&new_roboto_font_config,
		nullptr);
	
	return true;
}

PWSTR CDirectX::C_UI::open_file_dialog()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	PWSTR buff = nullptr;

    if (SUCCEEDED(hr))
    {
        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog,
                              nullptr,
                              CLSCTX_ALL,
                              IID_IFileOpenDialog,
                              reinterpret_cast<void**>(pFileOpen.GetAddressOf()));

        if (SUCCEEDED(hr))
        {
			DWORD options;
			hr = pFileOpen->GetOptions(&options);
			if (SUCCEEDED(hr))
			{
				hr = pFileOpen->SetOptions(options | FOS_PICKFOLDERS);

			}

            if (SUCCEEDED(hr))
            {

                if (SUCCEEDED(hr))
                {
                    // Show the Open dialog box.
                    hr = pFileOpen->Show(nullptr);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {

                        hr = pFileOpen->GetResult(pItem.GetAddressOf());
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
							{
								buff = pszFilePath;
                                CoTaskMemFree(pszFilePath);
                            }
                        }
                    }
                }
            }
        }
        CoUninitialize();
    }

	return buff;
}

std::string CDirectX::C_UI::Convert_PWSTR_To_String(const PWSTR pstr) {
    if (pstr == nullptr)
        return "";

    const int length = WideCharToMultiByte(CP_UTF8, 0, pstr, -1, nullptr, 0, nullptr, nullptr);
    if (length == 0)
        return "";

    std::string result(length - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, pstr, -1, &result[0], length, nullptr, nullptr);

    return result;
}

//Constructor of the CDirectX class, which init DirectX11
CDirectX::CDirectX(const HWND hWnd)
{
	//Creating a sd and filling it
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//Creating the actual device
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION,
		&sd, &pSwapChain, &pDevice, nullptr, &pDeviceContext);


	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView);
	pBackBuffer->Release();
}

//Destructor (Can be ignored)
CDirectX::~CDirectX()
{
	//Can be ignored because of the smart pointers (Com)

	//
	// if (pSwapChain != nullptr)
	// 	pSwapChain->Release();
	//
	// if (pDeviceContext != nullptr)
	// 	pDeviceContext->Release();
	//
	// if (pDevice != nullptr)
	// 	pDevice->Release();
}

//Rendering the Frames
void CDirectX::EndFrame() const
{
	pSwapChain->Present(1u, 0u);
}

CWindow::Exception::Exception(unsigned line, const char* file, HRESULT hResult) noexcept : Cexcept(line, file), hRresult(hResult)
{
}

const char* CWindow::Exception::what() const noexcept
{
	std::stringstream oss;
	oss << GetType() << std::endl << "[Error Code] " << GetErrorCode() << std::endl
	<< "[Description] " << GetErrorString() << std::endl << GetOrigString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* CWindow::Exception::GetType() const noexcept
{
	return "Window Exception";
}

std::string CWindow::Exception::TranslateErrorCode(HRESULT hResult) noexcept
{
	char* pMsgBuf = nullptr;
	const DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	                                    nullptr, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                                    reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
		return "Undefined Error Code";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT CWindow::Exception::GetErrorCode() const noexcept
{
	return hRresult;
}

std::string CWindow::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hRresult);
}

CWindow::CWindowClass::CWindowClass() noexcept : m_hInstance(GetModuleHandle(nullptr))
{
	//Defining the struct of WNDCLASSEX
	m_window_class.cbSize = sizeof(m_window_class);
	m_window_class.style = CS_OWNDC;
	m_window_class.lpfnWndProc = HandleMsgSetup;
	m_window_class.cbClsExtra = 0;
	m_window_class.cbWndExtra = 0;
	m_window_class.hInstance = get_Instance();
	m_window_class.hIcon = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, 0));
	m_window_class.hCursor = nullptr;
	m_window_class.hbrBackground = nullptr;
	m_window_class.lpszMenuName = nullptr;
	m_window_class.lpszClassName = get_name();
	m_window_class.hIconSm = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&m_window_class);
}

CWindow::CWindowClass::~CWindowClass()
{
	UnregisterClass(m_window_class_name, get_Instance());
}

HINSTANCE CWindow::CWindowClass::get_Instance() noexcept
{
	return m_window_class.hInstance;
}

const char* CWindow::CWindowClass::get_name() noexcept
{
	return m_window_class_name;
}

CWindow::CWindow(int width, int height, const char* name)
{
	//Calculate Window size
	RECT rect = {}; 

	rect.left = static_cast<LONG>(0.0f);
	rect.right = static_cast<LONG>(width);
	rect.top = static_cast<LONG>(0.0f);
	rect.bottom = static_cast<LONG>(height);




	if(FAILED(AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
		throw WND_LAST_EXCEPT();

	//Getting screen width etc..
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    //Calculate the position to center the window
	const int windowX = (screenWidth - (rect.right - rect.left)) / 2;
	const int windowY = (screenHeight - (rect.bottom - rect.top)) / 2;

    //Create Window
    hWnd = CreateWindowA(
		CWindowClass::get_name(),
        name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        windowX,
        windowY,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        CWindowClass::get_Instance(),
        this);


	if (hWnd == nullptr)
		throw WND_LAST_EXCEPT();

	//Title bar Black
	constexpr BOOL USE_DARK_MODE = true;
	if(FAILED(DwmSetWindowAttribute(
		hWnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
		&USE_DARK_MODE, sizeof(DWMWINDOWATTRIBUTE))))
		throw WND_LAST_EXCEPT();

	//Show window
	if (FAILED(ShowWindow(hWnd, SW_SHOWDEFAULT)))
		throw WND_LAST_EXCEPT();

	//DirectX  and ImGui
	pGfx = std::make_unique<CDirectX>( hWnd );
	pUI = std::make_unique<CDirectX::C_UI>(hWnd, *pGfx);
	
}

CWindow::~CWindow()
{
	DestroyWindow(hWnd);
}

WPARAM CWindow::Run() const noexcept
{
	MSG msg;

	while (true)
	{
		Sleep(1);

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			return msg.wParam;

		//Renders Frame (black)
		UI().Render(Gfx());
		Gfx().EndFrame();


	}

}

CDirectX& CWindow::Gfx() const
{
	return *pGfx;
}

CDirectX::C_UI& CWindow::UI() const
{
	return *pUI;
}

LRESULT CWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_NCCREATE)
	{
		const auto* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		auto* pHwnd = reinterpret_cast<CWindow*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd,  GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pHwnd));

		SetWindowLongPtr(hWnd,  GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));

		return pHwnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const auto pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags)
        {
            const RECT* suggested_rect = reinterpret_cast<RECT*>(lParam);
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
