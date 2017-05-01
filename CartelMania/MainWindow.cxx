#include "stdafx.h"
#include "MainWindow.h"
#include "banner.h"
#include "bannerline.h"
#include "debug.h"
#include "txtedit_dialog.h"
#include "ColorSelToolWnd.h"
#include "GlobalSettings.h"
#include "textrenderer.h"
#include "ColorComboBox.h"
#include "colors.h"

using namespace std;

// ---------------------------------------------------------------------------
// Globals/Externs
// ---------------------------------------------------------------------------
extern unique_ptr<Banner> g_curBanner;
extern GlobalSettings g_globalSettings;

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
	UpdateMenu();
	return 1L;
}

void CManiaMainWnd::DoPaint(CDCHandle hDC)
{	
	RECT rc;
	GetClientRect(&rc);
	g_curBanner->PaintOn(hDC, &rc);
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
			m_lineSelState.first = true;
			m_lineSelState.second = false;
			UpdateMenu();
			break;

		case ID_EDIT_SEL2:
			m_lineSelState.first = false;
			m_lineSelState.second  = true;
			UpdateMenu();
			break;

		case ID_EDIT_SELECTBOTH:
			m_lineSelState.first  = true;
			m_lineSelState.second  = true;
			UpdateMenu();
			break;
	}

	InvalidateRect(nullptr, FALSE);

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
	g_curBanner->Invalidate();
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDrawBoundingRects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	g_globalSettings.m_fDebugDrawBoundingRects = !g_globalSettings.m_fDebugDrawBoundingRects;
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
		g_curBanner->SetLayout(menuToLayoutMap.at(wID));
		InvalidateRect(nullptr, FALSE);
	}

	return 0L;
}


LRESULT CManiaMainWnd::OnSelectFx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{	
	switch (wID)
	{
		case ID_FX_SOLID: 
			ApplyFx<TextFxSolid>();
			break;

		case ID_FX_TWOOUTLINES: 
			ApplyFx<TextFxTwoOutlines>();
			break;
	
		case ID_FX_VERTICAL:
			break;

		case ID_FX_BLOCK:
			ApplyFx<TextFxBlock>();
			break;

		case ID_FX_SHADOWREAR:
			ApplyFx<TextFxShadowRear>();
			break;
	}

	// Notify the color Selection tool that the current fx style changed

	if (m_colorSelectToolWnd.m_hWnd)
	{
		m_colorSelectToolWnd.UpdateEntries();
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


template<class Fx_T>
void CManiaMainWnd::ApplyFx()
{
	if (m_lineSelState.first)
		g_curBanner->GetTopLine()->SetTextFx(make_unique<Fx_T>());

	if (m_lineSelState.second)
		g_curBanner->GetBottomLine()->SetTextFx(make_unique<Fx_T>());

	InvalidateRect(nullptr, FALSE);
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
			if (m_lineSelState.first && m_lineSelState.second)
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_CHECKED);
			}
			else if (m_lineSelState.first)
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_CHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_UNCHECKED);
			}
			else if (m_lineSelState.second)
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_CHECKED);
			}
		}

		HMENU hDebugMenu = GetSubMenu(hMenu, 7);
		if (hDebugMenu)
		{
			CheckMenuItem(hDebugMenu, ID_DEBUG_DRAWVERTICES, g_globalSettings.m_fDebugDrawVertices ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHFILL, g_globalSettings.m_fDebugDisableFillPath ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHSUBDIVISION, g_globalSettings.m_fDisableSubdiv ? MF_CHECKED : MF_UNCHECKED);
		}
	}
}

// ---------------------------------------------------------------------------
