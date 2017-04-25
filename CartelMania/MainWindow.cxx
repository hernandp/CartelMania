#include "stdafx.h"
#include "MainWindow.h"
#include "banner.h"
#include "bannerline.h"
#include "debug.h"
#include "txtedit_dialog.h"
#include "ColorSelToolWnd.h"
#include "globset.h"
#include "textrenderer.h"
#include "ColorComboBox.h"
#include "colors.h"

using namespace std;

// ---------------------------------------------------------------------------
// Globals/Externs
// ---------------------------------------------------------------------------
extern Banner g_curBanner;
extern GlobalSettings g_globalSettings;
extern bool g_lineSelState[2];

// ---------------------------------------------------------------------------

LRESULT CManiaMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT CManiaMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT CManiaMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{	
	//m_rebar.Create(m_hWnd, RECT{ 0, 0, 420, 50 }, 0, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | RBS_VARHEIGHT | RBS_BANDBORDERS | CCS_NODIVIDER);
	
	CmColorComboBox* cmcolor = new CmColorComboBox(&g_colorTable);
	HWND hwnd = cmcolor->Create(m_hWnd, RECT{ 0, 0, 400, 120 }, WS_CHILD | WS_VISIBLE);
	cmcolor->SetCurSel(4);
	UpdateMenu();

	return 1L;
}

LRESULT CManiaMainWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{	
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);
	RECT rc;
	GetClientRect(&rc);
	g_curBanner.PaintOn(hDC, &rc);
	EndPaint(&ps);
	return 0L;
}

LRESULT CManiaMainWnd::OnEditText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CTextEditDialog tdlg;
	tdlg.DoModal();

	return 0;
}

LRESULT CManiaMainWnd::OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (IDOK == (
		MessageBoxW(L"Do you want to exit CartelMania?", L"Exit", MB_ICONQUESTION | MB_OKCANCEL)))
	{
		CloseWindow(m_hWnd);
		DestroyWindow();
	}
	return 0L;
}

LRESULT CManiaMainWnd::OnEditSelLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	switch (wID)
	{
		case ID_EDIT_SEL1:
			g_lineSelState[0] = true;
			g_lineSelState[1] = false;
			UpdateMenu();
			break;

		case ID_EDIT_SEL2:
			g_lineSelState[0] = false;
			g_lineSelState[1] = true;
			UpdateMenu();
			break;

		case ID_EDIT_SELECTBOTH:
			g_lineSelState[0] = true;
			g_lineSelState[1] = true;
			UpdateMenu();
			break;
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDrawVertices(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	g_globalSettings.m_fDebugDrawVertices = !g_globalSettings.m_fDebugDrawVertices;
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDisablePathFill(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	g_globalSettings.m_fDebugDisableFillPath = !g_globalSettings.m_fDebugDisableFillPath;
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDisablePathSubdivision(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	g_globalSettings.m_fDisableSubdiv = !g_globalSettings.m_fDisableSubdiv;
	//g_curBanner.Invalidate();
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnSelectLayout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	static const map<int, BannerLayout> menuToLayoutMap{
	{ ID_LAYOUT_SINGLELINE, BannerLayout::SingleLine },
	{ ID_LAYOUT_LARGEOVERSMALL1, BannerLayout::LargeOverSmall1},
	{ ID_LAYOUT_LARGEOVERSMALL2, BannerLayout::LargeOverSmall2},
	{ ID_LAYOUT_LARGEOVERSMALL3, BannerLayout::LargeOverSmall3},
	{ ID_LAYOUT_MEDIUMOVERMEDIUM, BannerLayout::MediumMedium },
	{ ID_LAYOUT_SMALLOVERLARGE1, BannerLayout::SmallOverLarge1 },
	{ ID_LAYOUT_SMALLOVERLARGE2, BannerLayout::SmallOverLarge2 },
	{ ID_LAYOUT_SMALLOVERLARGE3, BannerLayout::SmallOverLarge3 }
	};

	// Handle layouts menu

	if (menuToLayoutMap.find(wID) != menuToLayoutMap.end())
	{
		g_curBanner.SetLayout(menuToLayoutMap.at(wID));
		InvalidateRect(nullptr, FALSE);
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnSelectFx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	switch (wID)
	{
		case ID_FX_SOLID:
			g_curBanner.GetTopLine()->SetTextFx(make_unique<TextFxSolid>());
			InvalidateRect(nullptr, FALSE);
			break;

		case ID_FX_THICK:
		{
			auto textR = make_unique<TextFxSolid>();
			textR->SetOutlineWidth(6.0f);
			g_curBanner.GetTopLine()->SetTextFx(move(textR));
			InvalidateRect(nullptr, FALSE);
			break;
		}

		case ID_FX_TWOOUTLINES:
		{
			auto textR = make_unique<TextFxTwoOutlines>();
			textR->SetOutlineWidth(6.0f);
			textR->SetOutline2Width(6.0f);
			g_curBanner.GetTopLine()->SetTextFx(move(textR));
			InvalidateRect(nullptr, FALSE);
			break;
		}

		case ID_FX_VERTICAL:
			break;

		case ID_FX_BLOCK:
			g_curBanner.GetTopLine()->SetTextFx(make_unique<TextFxBlock>());
			InvalidateRect(nullptr, FALSE);
			break;

		case ID_FX_SHADOWREAR:
			g_curBanner.GetTopLine()->SetTextFx(make_unique<TextFxShadowRear>());
			InvalidateRect(nullptr, FALSE);
			break;
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnColorOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	XASSERT(m_colorSelectToolWnd.Create(m_hWnd));
	m_colorSelectToolWnd.CenterWindow(m_hWnd);
	m_colorSelectToolWnd.ShowWindow(SW_SHOWNA);
	return 0L;
}

// ---------------------------------------------------------------------------

void CManiaMainWnd::UpdateMenu()
{
	HMENU hMenu = GetMenu();

	if (hMenu)
	{
		HMENU hEditMenu = GetSubMenu(hMenu, 1);
		if (hEditMenu)
		{
			if (g_lineSelState[0] && g_lineSelState[1])
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_CHECKED);
			}
			else if (g_lineSelState[0])
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_CHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_UNCHECKED);
			}
			else if (g_lineSelState[1])
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_CHECKED);
			}
		}

		HMENU hDebugMenu = GetSubMenu(hMenu, 6);
		if (hDebugMenu)
		{
			CheckMenuItem(hDebugMenu, ID_DEBUG_DRAWVERTICES, g_globalSettings.m_fDebugDrawVertices ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHFILL, g_globalSettings.m_fDebugDisableFillPath ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHSUBDIVISION, g_globalSettings.m_fDisableSubdiv ? MF_CHECKED : MF_UNCHECKED);
		}
	}
}

// ---------------------------------------------------------------------------
