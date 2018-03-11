#pragma once
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <xaudio2.h>
#include <string>

struct BaseInformation  //Basic Class
{
	HWND ViewportHWND;
	ID2D1Factory* D2D1Factory = NULL;
	IDWriteFactory* DWriteFactory = NULL;
	ID2D1HwndRenderTarget* D2D1HwndRenderTarget = NULL;
	IWICImagingFactory* WICImagingFactory = NULL;
	IDWriteTextFormat* BaseTextFormat = NULL;
	//struct DirectShowInfo
	//{
	//	IGraphBuilder *pGraph = NULL;
	//	IMediaControl *pControl = NULL;
	//	IMediaEvent   *pEvent = NULL;
	//	IMediaSeeking *pSeeking = NULL;
	//	IVMRWindowlessControl* pWindowLessControl = NULL;
	//} DirectShowInfo;
	IXAudio2* XAudio2Engine = NULL;
	IXAudio2MasteringVoice* XAudio2MasteringVoice = NULL;
};

extern BaseInformation Sys;
extern HWND MainHwnd;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

void InitEngine(HINSTANCE hInstance, int nShowCmd);
void InitGame();
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void DoLogic();
void DoRender();

class FFCmd
{

public:
	static bool IsCmdContain(std::string Cmd);
	static std::string GetCmdParam(std::string Cmd);
};