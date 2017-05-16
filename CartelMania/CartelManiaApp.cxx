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

Gdiplus::Size CartelManiaApp::GetPaperSize() const
{
	Gdiplus::Size paperSize(0, 0);

	CDevMode dm;
	if (dm.CopyFromPrinter(m_printer.m_hPrinter))
	{
		paperSize.Height = dm.m_pDevMode->dmPaperLength;
		paperSize.Width  = dm.m_pDevMode->dmPaperWidth;
	}
	return paperSize;
}

CartelManiaApp::~CartelManiaApp()
{
	dprintf(__FUNCTIONW__);
}

int CartelManiaApp::Run()
{
	m_settings.Load();

	if (!m_printer.OpenDefaultPrinter())
	{
		MessageBox(0,
			L"Cannot open your default printer.\n\nCartelMania requires an installed printer to run. ",
			L"CartelMania",
			MB_OK | MB_ICONERROR);
		return GetLastError();
	}

	// We can set the default banner dimensions: One page of the current selected paper on the
	// default printer.

	CDevMode dm;
	if (!dm.CopyFromPrinter(m_printer.m_hPrinter))
	{
		MessageBox(0,
			L"Error accessing your printer settings. Verify your printer configuration and installation and try again.",
			L"CartelMania",
			MB_OK | MB_ICONERROR);
		return GetLastError();
	}

	m_banner.SetSizeMm(Size(dm.m_pDevMode->dmPaperWidth / 10, dm.m_pDevMode->dmPaperLength / 10));
	
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
