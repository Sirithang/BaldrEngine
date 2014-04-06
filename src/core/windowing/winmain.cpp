#ifdef _WIN64

#include <windows.h>
#include <windowsx.h>

#include "core/windowing.h"

using namespace baldr;


struct WinHandleWin64 : public WinHandle
{

};

WinHandle* createWindowWin(int, char**);
void showWindowWin(WinHandle*);
void updateWindowWin(WinHandle*);

WinHandle* (*window::createWindow)(int, char**) = createWindowWin;
extern void (*window::showWindow)(WinHandle*) = showWindowWin;
extern void (*window::updateWindow)(WinHandle*) = updateWindowWin;


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//=========================================

WinHandle* createWindowWin(int argc, char** argv)
{
	WinHandleWin64* handle = new WinHandleWin64();
	handle->_opened = false;

    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, 800, 600};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	handle->nativeHandle = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"BaldrEngine",
                          WS_OVERLAPPEDWINDOW,
                          300,
                          300,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          GetModuleHandle(NULL),
                          NULL);

	return handle;
}

//***********************

void showWindowWin(WinHandle* handle)
{
	WinHandleWin64* winh = (WinHandleWin64*)handle;

	handle->_opened = true;
	ShowWindow((HWND)winh->nativeHandle, SW_SHOW);
}

//**********************

void updateWindowWin(WinHandle* handle)
{
	MSG msg;
	WinHandleWin64* win = (WinHandleWin64*)handle;

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if(msg.message == WM_QUIT)
			win->_opened = false;
    }
}



//**********************************************************

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

#endif