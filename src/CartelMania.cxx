#include "stdafx.h"
#include "CartelManiaApp.h"
#include "GdiPEngWrapper.h"

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
// 
// Application entry-point
//
//----------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	GdiPlusEngine gdiplus;
	XASSERT(gdiplus.IsInitOk());

	AtlInitCommonControls(ICC_WIN95_CLASSES);
	CartelManiaApp app;
	return app.Run();
}
