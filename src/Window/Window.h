#pragma once

#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <sstream>
#include <shobjidl.h> 
#include "Cexcept.h"
#include <shellapi.h>
#include <thread>

#include "../../ext/ImGui/imgui.h"
#include "../../ext/ImGui/imgui_impl_win32.h"
#include "../../ext/ImGui/imgui_impl_dx11.h"


#include "../Backend/Image/ImageMaker.h"
#include "../Settings.h"

#include "../../ext/Font/New Athletic M54.h"
#include "../../ext/Font/aAbsoluteEmpire.h"
#include "../../ext/Font/RobotoRegular.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dwmapi.lib")


static constexpr const char* app_name_release = "Image Converter v1.0  -  Release";
static constexpr const char* app_name_debug = "Image Converter v1.0  -  Debug";

static void HelpMarker(const char* desc);
bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f);
void AlignForWidth(float width, float alignment = 0.5f);


class CDirectX
{
public:

	class C_UI
	{
	public:
		explicit C_UI(const HWND& hWnd, const CDirectX& dxInstance) noexcept;
		~C_UI();



		void Render(CDirectX& DirectX_Cui_instance);
		bool set_WindowSize();


		void set_style();
		bool init_fonts();

		PWSTR open_file_dialog();
		std::string Convert_PWSTR_To_String(const PWSTR pstr);

	private:
		ImGuiContext* pImGuiContext = nullptr;
		const int ImGui_Flags = (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		std::unique_ptr<char[]> font1;


		Microsoft::WRL::ComPtr<IFileOpenDialog> pFileOpen;
		Microsoft::WRL::ComPtr<IShellItem> pItem;

		std::unique_ptr<unsigned char[]> pNew_Athletic_font_normal;
		std::unique_ptr<unsigned char[]> pNew_Absolute_empire_font_Headers;
		std::unique_ptr<unsigned char[]> pNew_Athletic_font_normal_small;
		std::unique_ptr<unsigned char[]> pNew_Roboto_font;

		ImFont* font_headers;
		ImFont* font_else;
		ImFont* font_else_small;
		ImFont* font_roboto;
	};




	CDirectX(HWND hWnd);
	CDirectX( const CDirectX& ) = delete;
	CDirectX& operator=(const CDirectX &) = delete;
	~CDirectX();

	void EndFrame() const;


private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;

};

class CWindow
{

	class Exception : public Cexcept
	{
	public:
		Exception(unsigned int line, const char* file, HRESULT hResult) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hResult) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hRresult;

	};


	class CWindowClass
	{
	private:
		static HWND m_hwnd;
		static WNDCLASSEX m_window_class;
		HINSTANCE m_hInstance;


		static constexpr const char* m_window_class_name = "Image Converter";
		
		static CWindowClass wnd_class_;

		CWindowClass() noexcept;
		CWindowClass( const CWindowClass& ) = delete;
		CWindowClass& operator=(const CWindowClass&) = delete;
		~CWindowClass();

	public:
		static HINSTANCE get_Instance() noexcept;
		static const char* get_name() noexcept; 
	};

public:
	CWindow(int width, int height, const char* name);
	~CWindow();
	CWindow( const CWindow& ) = delete;
	CWindow& operator=(const CWindow& ) = delete;
	WPARAM Run() const noexcept;

	CDirectX& Gfx() const;
	CDirectX::C_UI& UI() const;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const noexcept;

	int width;
	int height;
	HWND hWnd;

	std::unique_ptr<CDirectX> pGfx;
	std::unique_ptr<CDirectX::C_UI> pUI;
};