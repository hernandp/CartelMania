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

inline CartelManiaApp::~CartelManiaApp()
{
	dprintf(__FUNCTIONW__);
}

int CartelManiaApp::Run()
{
	m_settings.Load();

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
