#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <gdiplus.h>
#include <string>
#include "resource.h"
#include "cmania.h"
#include "debug.h"

// Use VisualLeak Detector 
#include <vld.h>

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
void ExecMenu(HWND, int);
void UpdateMenu(HWND);

//----------------------------------------------------------------------------
// 
// Application entry-point
//
//----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	XASSERT(g_gdipEng.IsInitOk());

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

	XASSERT(RegisterClassEx(&wcex));

	HWND hWnd = NULL;
	XASSERT(hWnd = CreateWindowEx(0, g_WindowClassName, g_WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, 0));

	MSG msg;
	RtlZeroMemory(&msg, sizeof(MSG));
	
	if (hWnd)
	{
		BOOL bRet;

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);
		
		while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
		{
			XASSERT(bRet != -1);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
//----------------------------------------------------------------------------

static BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	UpdateMenu(hwnd);
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
