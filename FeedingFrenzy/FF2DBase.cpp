#include "FF2DBase.h"
#include <chrono>
#include "FFReflectionManager.h"
#include "GlobalMacro.h"
#include "FF2DOperations.h"
#include "FFGCManager.h"
#include "FFTimerManager.h"
#include "FFRenderManager.h"
#include <algorithm>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"xaudio2.lib")

BaseInformation Sys = { 0 };

inline void CheckAndPostErrorMsg(bool Value, const WCHAR* ErrorMessage)
{
	if (Value)
	{
		MessageBox(NULL, ErrorMessage, TEXT("Error"), MB_OK);
		exit(-1);
	}
}

std::chrono::steady_clock::time_point LastTimePoint = std::chrono::steady_clock::now();

#ifdef _WIN32
HWND MainHwnd;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	FFReflectionManager::Get().ModuleMap.insert_or_assign("__FF_ENGINE", hInstance);
	InitEngine(hInstance, nShowCmd);
	InitGame();
	MSG msg = { NULL };
	while (msg.message != WM_QUIT)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		GetMessage(&msg, 0, 0, 0);
	}
	return 0;
}
#endif // _WIN32

void InitGame()
{
	std::string ErrorMsg;
	MACRO_SPLICE(OPERATION_PREFIX, LoadJson)(json11::Json::parse("{FileName:\"Init.json\"}", ErrorMsg));
	if (!ErrorMsg.empty())
	{
		debug << "Fatal Error:Init.Json Not Found,Exit!";
		MessageBox(MainHwnd, TEXT("Fatal Error:Init.Json Not Found,Exit!"), TEXT("Error!"), MB_OK);
		exit(-1);
	}
}

void InitEngine(HINSTANCE hInstance, int nShowCmd)
{
	//Begin Init Wndow for Game Viewport
	RECT WndRC = { 0,0,384,768 };
	WNDCLASSEX wnd = { NULL };
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hIcon = LoadIcon(0, IDC_ICON);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = MainWndProc;
	wnd.lpszClassName = TEXT("RAIDEN ENGINE VIEWPORT");
	RegisterClassEx(&wnd);
	AdjustWindowRect(&WndRC, WS_OVERLAPPEDWINDOW, FALSE);
	Sys.ViewportHWND = CreateWindow(wnd.lpszClassName, TEXT("RAIDEN ENGINE"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WndRC.right - WndRC.left, WndRC.bottom - WndRC.top, NULL, NULL, hInstance, NULL);
	CheckAndPostErrorMsg(!Sys.ViewportHWND, TEXT("Create Window as Viewport Failed,Exit..."));
	ShowWindow(Sys.ViewportHWND, nShowCmd);
	//End Init Window For Game Viewport
	setlocale(LC_ALL, "");
	//Begin Init Direct2D
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Sys.D2D1Factory);
	CheckAndPostErrorMsg(!Sys.D2D1Factory, TEXT("D2D1Factory Failed to Create,Exit..."));
	Sys.D2D1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT), D2D1::HwndRenderTargetProperties(Sys.ViewportHWND, D2D1::SizeU(WndRC.right - WndRC.left, WndRC.bottom - WndRC.top)), &Sys.D2D1HwndRenderTarget);
	CheckAndPostErrorMsg(!Sys.D2D1HwndRenderTarget, TEXT("D2D1 RenderTarget Failed to Create,Exit..."));
	//End Init Direct2D
	//Begin Init WICImageComponent
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&Sys.WICImagingFactory));
	CheckAndPostErrorMsg(!Sys.WICImagingFactory, TEXT("WICImageComponent Failed to Create,Exit..."));
	//End Init WICImageComponent
	//Begin Init DWrite
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&Sys.DWriteFactory));
	Sys.DWriteFactory->CreateTextFormat(TEXT("Î¢ÈíÑÅºÚ"), nullptr,
										DWRITE_FONT_WEIGHT_REGULAR,
										DWRITE_FONT_STYLE_NORMAL,
										DWRITE_FONT_STRETCH_NORMAL, 20, TEXT("zh-cn"), &Sys.BaseTextFormat);
	CheckAndPostErrorMsg(!Sys.BaseTextFormat, TEXT("Base Text Format Failed to Create,Exit..."));
	//End Init DWrite
	//Begin Init XAudio2
	CheckAndPostErrorMsg(FAILED(XAudio2Create(&Sys.XAudio2Engine)), TEXT("XAudio2 Component Failed To Create,Exit..."));
	CheckAndPostErrorMsg(FAILED(Sys.XAudio2Engine->CreateMasteringVoice(&Sys.XAudio2MasteringVoice)), TEXT("XAudio2 MasteringVoice Failed To Create,Exit..."));
	//End Init XAudio2
	////Begin Init DShow
	//CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&Sys.DirectShowInfo.pGraph);
	//Sys.DirectShowInfo.pGraph->QueryInterface(IID_IMediaControl, (void **)&Sys.DirectShowInfo.pControl);
	//Sys.DirectShowInfo.pGraph->QueryInterface(IID_IMediaEvent, (void **)&Sys.DirectShowInfo.pEvent);
	//Sys.DirectShowInfo.pGraph->QueryInterface(IID_IMediaSeeking, (void **)&Sys.DirectShowInfo.pSeeking);
	//InitWindowlessVMR(Sys.ViewportHWND, Sys.DirectShowInfo.pGraph, &Sys.DirectShowInfo.pWindowLessControl);
	////End Init DShow
	//Set Timer For FrameTick
	SetTimer(MainHwnd, 0, 0, nullptr);
	//InitGCManager
	FFGCManager::Get();
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case  WM_TIMER:
			{
				DoLogic();
				DoRender();
			}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void DoLogic()
{
	auto timeP = std::chrono::steady_clock::now();
	int millisecond = (int)std::chrono::duration_cast<std::chrono::milliseconds>(timeP - LastTimePoint).count();
	LastTimePoint = timeP;
	FFTimerManager::Get().Tick(millisecond);
}

void DoRender()
{
	FFRenderManager::Get().NotifyRender();
}

bool FFCmd::IsCmdContain(std::string Cmd)
{
	std::transform(Cmd.begin(), Cmd.end(), Cmd.begin(), tolower);
	for (int i = 0; i < __argc; i++)
	{
		std::string CompaireCmd(__argv[i]);
		std::transform(CompaireCmd.begin(), CompaireCmd.end(), CompaireCmd.begin(), tolower);
		if (Cmd == CompaireCmd)
		{
			return true;
		}
	}
	return false;
}

std::string FFCmd::GetCmdParam(std::string Cmd)
{
	std::transform(Cmd.begin(), Cmd.end(), Cmd.begin(), tolower);
	for (int i = 0; i < __argc - 1; i++)
	{
		std::string CompaireCmd(__argv[i]);
		std::transform(CompaireCmd.begin(), CompaireCmd.end(), CompaireCmd.begin(), tolower);
		if (Cmd == CompaireCmd)
		{
			return __argv[i + 1];
		}
	}
	return std::string();
}
