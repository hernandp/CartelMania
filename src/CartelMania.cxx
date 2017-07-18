/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
#include "stdafx.h"
#include "CartelManiaApp.h"
#include "GdiPEngWrapper.h"

// Use VisualLeak Detector 
// #include <vld.h>
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
