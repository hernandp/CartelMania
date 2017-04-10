#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <gdiplus.h>
#include <string>
#include "resource.h"
#include "cmania.h"
#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

//----------------------------------------------------------------------------
// 
// Globals
//
//----------------------------------------------------------------------------
const WCHAR g_WindowClassName[] = L"CartelManiaWindowClass";
const WCHAR g_WindowTitle[] = L"CartelMania for Windows";
GdiPlusEngine g_gdipEng;
Banner g_curBanner;
bool g_lineSelState[2]{ true,true };

//----------------------------------------------------------------------------
// 
// Function Prototypes
//
//----------------------------------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void FatalMsg(HWND hWnd, DWORD dwErrCode, LPCWSTR lpMessage);
void ExecMenu(HWND, int);

//----------------------------------------------------------------------------
// 
// Application entry-point
//
//----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (!g_gdipEng.IsInitOk())
	{
		FatalMsg(NULL, 0, L"GDI+ initialization error");
		return -1;
	}

	WNDCLASSEX wcex;
	RtlZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpfnWndProc	= WindowProc;
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
	wcex.hInstance		= hInstance;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName	= g_WindowClassName;
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	if (!RegisterClassEx(&wcex))
	{
		FatalMsg(NULL, GetLastError(), L"Initialization error registering window class");
		return -1;
	}

	HWND hWnd = CreateWindowEx(0, g_WindowClassName, g_WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, 0);

	MSG msg;
	RtlZeroMemory(&msg, sizeof(MSG));
	
	if (hWnd)
	{
		BOOL bRet;

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);
		
		while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
		{
			if (bRet == -1)
			{
				FatalMsg(hWnd, GetLastError(), L"Unexpected message loop error");
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	else
	{
		FatalMsg(hWnd, GetLastError(), L"Initialization error creating window");
	}

	return (int) msg.wParam;
}
//----------------------------------------------------------------------------

static BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HMENU hMenu = GetMenu(hwnd);
	if (hMenu)
	{
		
	}

	return TRUE;
}

//----------------------------------------------------------------------------

static void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);

	RECT rc;
	GetClientRect(hWnd, &rc);
	g_curBanner.PaintOn(hDC, &rc);
	EndPaint(hWnd, &ps);
}

//----------------------------------------------------------------------------

static void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (codeNotify == 0)
	{
		ExecMenu(hWnd, id);
	}
}

//----------------------------------------------------------------------------

void OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

//----------------------------------------------------------------------------

void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

//----------------------------------------------------------------------------
// 
// Window Procedure
//
//----------------------------------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

//----------------------------------------------------------------------------

static void FatalMsg(HWND hWnd, DWORD dwErrCode, LPCWSTR lpMessage)
{
	WCHAR wMsg[1024] = { 0 };
	swprintf_s(wMsg, L"%s\n\n(Code=%d)", lpMessage, dwErrCode);
	MessageBox(hWnd, wMsg, L"Fatal Error", MB_ICONERROR | MB_OK);
}
