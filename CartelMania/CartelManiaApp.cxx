#include "stdafx.h"
#include "CartelManiaApp.h"
#include "debug.h"
#include "colorTable.h"

using namespace std;
using namespace Gdiplus;

CartelManiaApp* CartelManiaApp::s_appPtr = nullptr;

// ---------------------------------------------------------------------------
CartelManiaApp::CartelManiaApp()
{
	dprintf(__FUNCTIONW__);
	XASSERT(!s_appPtr);
	s_appPtr = this;
		
	m_mainWindow = std::make_unique<CManiaMainWnd>();
}

Gdiplus::Size CartelManiaApp::GetPaperSizeMm() const
{
	Gdiplus::Size paperSize(0, 0);

	CDevMode dm;
	if (dm.CopyFromPrinter(m_printer.m_hPrinter))
	{
		paperSize.Height = dm.m_pDevMode->dmPaperLength / 10;
		paperSize.Width  = dm.m_pDevMode->dmPaperWidth / 10;
	}
	return paperSize;
}

Gdiplus::Size CartelManiaApp::GetPrintableAreaMm() const
{
	CDC printerDC(m_printer.CreatePrinterDC());
	int dpiX = printerDC.GetDeviceCaps(LOGPIXELSX);
	int dpiY = printerDC.GetDeviceCaps(LOGPIXELSY);
	int cxPrintableArea = printerDC.GetDeviceCaps(HORZRES);
	int cyPrintableArea = printerDC.GetDeviceCaps(VERTRES);
	float cxPrintableAreaMm = cxPrintableArea / dpiX  * 25.4f;
	float cyPrintableAreaMm = cyPrintableArea / dpiY  * 25.4f;

	if (m_devMode.m_pDevMode->dmOrientation == DMORIENT_PORTRAIT)
	{
		return Size((int) cxPrintableAreaMm, (int) cyPrintableAreaMm);
	}
	else //DMORIENT_LANDSCAPE
	{
		return Size((int) cyPrintableAreaMm, (int) cxPrintableAreaMm);
	}
}

int CartelManiaApp::GetPaperOrientation() const
{
	CDevMode dm;
	if (dm.CopyFromPrinter(m_printer.m_hPrinter))
	{
		return dm.m_pDevMode->dmOrientation;
	}

	return -1;
}

std::wstring CartelManiaApp::GetPaperName() const
{
	/*CDevMode dm;
	if (dm.CopyFromPrinter(m_printer.m_hPrinter))
	{
		dm.m_pDevMode->dmMediaType
	}*/

	return L"";
}

CartelManiaApp::~CartelManiaApp()
{
	dprintf(__FUNCTIONW__);
}

int CartelManiaApp::Run()
{
	m_settings.Load();

	DWORD dwPrintErr;
	if (dwPrintErr = (SetupPrinter() != ERROR_SUCCESS))
	{
		wchar_t msg[255];
		StringCchPrintf(msg, _countof(msg), L"Cannot configure your default printer.\n"
			L"CartelMania requires an installed and properly configured printer to run.\n"
			L"\nWin32 error code = %d", dwPrintErr);

		MessageBox(0, msg, L"CartelMania",	MB_OK | MB_ICONERROR);
		return dwPrintErr;
	}

	// Main window setup and message loop

	HMENU hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	XASSERT(m_mainWindow->Create(0, CWindow::rcDefault,  L"CartelMania for Windows", 0, 0, hMenu));

	MSG msg;
	if (m_mainWindow->m_hWnd)
	{
		m_mainWindow->ShowWindow(SW_SHOWDEFAULT);
		m_mainWindow->UpdateWindow();

		BOOL bRet;
		while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0)
		{
			XASSERT(bRet != -1);

			if (!IsDialogMessage(*m_mainWindow, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	// Exiting ...

	m_settings.Save();
	return static_cast<int>(msg.wParam);
}

DWORD CartelManiaApp::SetupPrinter()
{
	if (m_printer.OpenDefaultPrinter())
	{
		CDevMode dm;
		if (dm.CopyFromPrinter(m_printer.m_hPrinter))
		{
			// Orient by default as landscape
			dm.m_pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
			HANDLE hDevMode = dm.CopyToHDEVMODE();

			if (hDevMode == NULL)
				return ERROR_NOT_ENOUGH_MEMORY;

			if (!m_devMode.CopyFromHDEVMODE(hDevMode))
				return ERROR_NOT_ENOUGH_MEMORY;

			m_printer.ClosePrinter();

			if (!m_printer.OpenDefaultPrinter(m_devMode))
			{
				GlobalFree(hDevMode);
				return GetLastError();
			}

			GlobalFree(hDevMode);
		}
		
		return ERROR_SUCCESS;
	}

	return GetLastError();
}
