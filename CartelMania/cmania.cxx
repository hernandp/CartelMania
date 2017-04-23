#include <atlbase.h>
#include <atlwin.h>
#include <gdiplus.h>
#include "resource.h"
#include "cmania.h"
#include "mainwnd.h"

// Use VisualLeak Detector 
#include <vld.h>

using namespace std;
using namespace Gdiplus;

//----------------------------------------------------------------------------
// Linker directives
//----------------------------------------------------------------------------
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//----------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------
const WCHAR g_WindowTitle[] = L"CartelMania for Windows";
GdiPlusEngine g_gdipEng;
Banner g_curBanner;
GlobalSettings g_globalSettings;
bool g_lineSelState[2]{ true,true };

//----------------------------------------------------------------------------
// 
// Application entry-point
//
//----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	AtlInitCommonControls(ICC_WIN95_CLASSES);
	XASSERT(g_gdipEng.IsInitOk());
	
	CManiaMainWnd mainWindow;
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	XASSERT(mainWindow.Create(0, CWindow::rcDefault, g_WindowTitle, 0, 0, hMenu));

	MSG msg;
	if (mainWindow.m_hWnd)
	{
		mainWindow.ShowWindow(SW_SHOWDEFAULT);
		mainWindow.UpdateWindow();

		BOOL bRet;
		while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
		{
			XASSERT(bRet != -1);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}	
	return (int) msg.wParam;
}
