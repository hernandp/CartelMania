#include "stdafx.h"
#include "CartelManiaApp.h"
#include "debug.h"
#include "colors.h"

CartelManiaApp * CartelManiaApp::m_inst = nullptr;

CartelManiaApp::CartelManiaApp()
{
	XASSERT(m_gdipEng.IsInitOk());
	m_mainWindow = std::make_unique<CManiaMainWnd>();
}

int CartelManiaApp::Run()
{
	InitColorTable();

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
	return msg.wParam;
}
