#include "stdafx.h"
#include "MainWindow.h"
#include "banner.h"
#include "bannerline.h"
#include "debug.h"
#include "TextEditToolWnd.h"
#include "ColorSelToolWnd.h"
#include "AppSettings.h"
#include "TextFx.h"
#include "ColorComboBox.h"
#include "colorTable.h"
#include "CartelManiaApp.h"

using namespace std;

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
	XASSERT(m_imgList.Create(24,24, ILC_COLOR16|ILC_MASK, 10, 10));
	XASSERT(m_toolbar.Create(*this, 0, nullptr, WS_VISIBLE | WS_CHILD));
	m_toolbar.SetImageList(m_imgList, 0);
	m_toolbar.LoadStdImages(IDB_STD_LARGE_COLOR);
	 
	const DWORD buttonStyles = BTNS_AUTOSIZE;
	TBBUTTON tbButtons[] =
	{
		{ MAKELONG(STD_FILENEW,  0),  ID_COLOR_OPEN,  TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 },
		{ MAKELONG(STD_FILESAVE,  0), ID_CMD_EDITTEXT,  TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 },
		{ MAKELONG(STD_FIND,      0), ID_CMD_OPENSHAPETOOL, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 },
		{ MAKELONG(STD_PRINTPRE,  0), ID_CMD_PRINTPRE, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 },
		{ MAKELONG(STD_PRINT,     0), ID_CMD_PRINT, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 }	
	};
	m_toolbar.SetButtonStructSize(sizeof(TBBUTTON));
	m_toolbar.AddButtons(_countof(tbButtons), tbButtons);
	m_toolbar.AutoSize();

	UpdateMenu();
	return 1L;
}

void CManiaMainWnd::DoPaint(CDCHandle hDC)
{	
	DrawClientArea(hDC);

	RECT rc;
	GetClientRect(&rc);

	auto banner = App()->GetBanner();
	banner->PaintOn(hDC, &rc);

	// Selection marks
	//
	Gdiplus::Graphics gr(hDC);

	Gdiplus::RectF line1Rect, line2Rect;
	banner->GetLineRects(banner->GetRect(&rc), line1Rect, line2Rect);

	gr.TranslateTransform(rc.left + BANNER_MARGIN_PX / 2, (rc.bottom-rc.top) / 2.0f - banner->GetRect(&rc).Height / 2.0f);

	if (App()->GetMainWindow()->GetLineSelState().first || App()->GetBanner()->GetLayout() == BannerLayout::SingleLine)
		DrawSelectionMark(gr, line1Rect);

	if (App()->GetMainWindow()->GetLineSelState().second && App()->GetBanner()->GetLayout() != BannerLayout::SingleLine)
	{
		gr.TranslateTransform(0, line1Rect.Height);
		DrawSelectionMark(gr, line2Rect);
	}
}

void CManiaMainWnd::DrawSelectionMark(Gdiplus::Graphics &gr, const Gdiplus::RectF& rect)
{
	const Gdiplus::Pen selMarkerPen(Gdiplus::Color::Green, 3);
	const float margin_L = 8.0f;
	const float margin_R = 8.0f;
	const float margin_T = 8.0f;
	const float margin_B = 8.0f;
	const float bracket_len = 24.0f;

	Gdiplus::PointF bracketL[]{
		{bracket_len,margin_L},
		{margin_L,margin_T},
		{margin_L, rect.Height - margin_B},
		{ bracket_len, rect.Height - margin_B } };

	Gdiplus::PointF bracketR[]{
		{ rect.Width - margin_R - bracket_len, margin_T },
		{ rect.Width - margin_R , margin_T },
		{ rect.Width - margin_R, rect.Height - margin_B},
		{ rect.Width - margin_R - bracket_len, rect.Height - margin_B }
	};

	gr.DrawLines(&selMarkerPen, bracketL, _countof(bracketL));
	gr.DrawLines(&selMarkerPen, bracketR, _countof(bracketR));
}

void CManiaMainWnd::DrawClientArea(CDCHandle hDC)
{
	RECT rc;
	GetClientRect(&rc);

	const Gdiplus::RectF rcClientArea((float) rc.left, (float) rc.top, float(rc.right - rc.left), float(rc.bottom - rc.top));
	Gdiplus::Graphics gr(hDC);

	// Background

	gr.FillRectangle(&Gdiplus::HatchBrush(Gdiplus::HatchStyle10Percent, Gdiplus::Color::Blue, Gdiplus::Color::DarkCyan), rcClientArea);
	

	// Transform coordinate space.
	// Banner area spans over 100% width minus borders, and 50% height, of the window client area.
	// Origin of drawing is set to the upper-left corner of the banner, with Y+ pointing down, X+ pointing left.
	// We still use pixel units, however.
	//
	const Gdiplus::RectF bannerRect(0, 0, rcClientArea.Width - BANNER_MARGIN_PX,
		rcClientArea.Height * BANNER_HEIGHT_PCT / 100.0f);

	gr.TranslateTransform(rcClientArea.X + BANNER_MARGIN_PX / 2, rcClientArea.Height / 2.0f - bannerRect.Height / 2.0f);
	gr.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color::White), bannerRect);

	// Draw banner page and shadow

	Gdiplus::RectF bannerShadowRect = bannerRect;
	bannerShadowRect.Offset(4.0f, 4.0f);
	gr.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color::Black), bannerShadowRect);
	gr.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color::Black), bannerRect);
}

LRESULT CManiaMainWnd::OnEditText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (!m_textEditToolWnd.m_hWnd)
		XASSERT(m_textEditToolWnd.Create(m_hWnd));

	auto lastX = App()->GetSettings()->lastTextEditToolPos.x;
	auto lastY = App()->GetSettings()->lastTextEditToolPos.y;

	if (lastX == -1 && lastY == -1)
	{
		// Position default 
		lastX = 0;
		lastY = 0;
	}

	m_textEditToolWnd.SetWindowPos(nullptr, lastX, lastY, -1, -1, SWP_NOZORDER | SWP_NOSIZE);	
	m_textEditToolWnd.ShowWindow(SW_SHOWNA);
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
	App()->GetSettings()->debugDrawVertices = !App()->GetSettings()->debugDrawVertices;
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDisablePathFill(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	App()->GetSettings()->debugDisableFillPath = !App()->GetSettings()->debugDisableFillPath;
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDisablePathSubdivision(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	App()->GetSettings()->debugDisableSubdiv = !App()->GetSettings()->debugDisableSubdiv;
	App()->GetBanner()->Invalidate();
	InvalidateRect(nullptr, FALSE);
	UpdateMenu();
	return 0L;
}

LRESULT CManiaMainWnd::OnDebugDrawBoundingRects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	App()->GetSettings()->debugDrawBoundingRects = !App()->GetSettings()->debugDrawBoundingRects;
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

	XASSERT(menuToLayoutMap.find(wID) != menuToLayoutMap.end());

	auto currentLayout = App()->GetBanner()->GetLayout();
	auto selectedLayout = menuToLayoutMap.at(wID);

	// Handle layouts menu  (do nothing if layout is the same as selected)

	if (currentLayout != selectedLayout)
	{
		if (m_textEditToolWnd.m_hWnd &&
			(!AreCompatibleLayouts(currentLayout, selectedLayout)))
		{
			m_textEditToolWnd.LayoutUpdate(selectedLayout);
		}

		App()->GetBanner()->SetLayout(selectedLayout);
		InvalidateRect(nullptr, FALSE);
	}

	return 0L;
}


LRESULT CManiaMainWnd::OnSelectFx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{	
	switch (wID)
	{
		case ID_FX_SOLID:			ApplyFx<TextFxSolid>();			break;
		case ID_FX_TWOOUTLINES:		ApplyFx<TextFxTwoOutlines>();	break;
		case ID_FX_VERTICAL:		break;
		case ID_FX_BLOCK:			ApplyFx<TextFxBlock>();			break;
		case ID_FX_SHADOWREAR:		ApplyFx<TextFxShadow>(ShadowType::Rear); break;
		case ID_FX_SHADOWFORE:		ApplyFx<TextFxShadow>(ShadowType::Fore); break;
	}

	InvalidateRect(nullptr, FALSE);

	if (m_colorSelectToolWnd.m_hWnd)
	{
		m_colorSelectToolWnd.UpdateUI();
	}
	return 0L;
}

LRESULT CManiaMainWnd::OnLayoutScaleToFit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	App()->GetBanner()->SetScalePolicy(ScalePolicy::ScaleToFit);
	return 0;
}

template <class T, typename ...U> 
void CManiaMainWnd::ApplyFx(U... v)
{
	if (m_lineSelState.first)
		App()->GetBanner()->GetTopLine()->SetTextFx(make_unique<T>(v...));

	if (m_lineSelState.second)
		App()->GetBanner()->GetBottomLine()->SetTextFx(make_unique<T>(v...));
}

LRESULT CManiaMainWnd::OnColorOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (m_colorSelectToolWnd.m_hWnd == nullptr)
		XASSERT(m_colorSelectToolWnd.Create(m_hWnd));

	m_colorSelectToolWnd.ShowWindow(SW_SHOWNA);
	return 0L;
}

LRESULT CManiaMainWnd::OnOpenShapeTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	auto lastX = App()->GetSettings()->lastShapeEditToolPos.x;
	auto lastY = App()->GetSettings()->lastShapeEditToolPos.y;
	auto lastSizeX = App()->GetSettings()->lastShapeEditToolSize.x;
	auto lastSizeY = App()->GetSettings()->lastShapeEditToolSize.y;

	if (lastX == -1 && lastY == -1)
	{
		// Position defaults
		lastX = 0;
		lastY = 0;
	}

	if (lastSizeX == -1 && lastSizeY == -1)
	{
		// Size defaults
		lastSizeX = 150;
		lastSizeY = 275;
	}

	if (m_shapeSelectToolWnd.m_hWnd == nullptr)
		XASSERT(m_shapeSelectToolWnd.Create(m_hWnd, RECT{ lastX,lastY,lastX + lastSizeX,lastY + lastSizeY }));

	m_shapeSelectToolWnd.SetWindowPos(nullptr, lastX, lastY, lastSizeX, lastSizeY, SWP_NOZORDER);
	m_shapeSelectToolWnd.ShowWindow(SW_SHOWNA);
	return 0L;
}


LRESULT CManiaMainWnd::OnPrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPrintJob job;
	CPrintDialogEx printDlg;
	printDlg.DoModal();

	CPrinter printer;
	if (printer.OpenPrinterW(printDlg.GetDeviceName(), printDlg.GetDevMode()))
	{
		job.StartPrintJob(false, printer.m_hPrinter, printDlg.GetDevMode(), &m_printJobInfo, L"xxx", 0, 0);
	}
	return 0L;
}

LRESULT CManiaMainWnd::OnPrintPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPrintDialogEx printDlg;
	printDlg.DoModal();
	CPrinter printer;
	if (printer.OpenPrinterW(printDlg.GetDeviceName(), printDlg.GetDevMode()))
	{
		CPrintPreviewWindow* prnPreWnd = new CPrintPreviewWindow;
		prnPreWnd->SetPrintPreviewInfo(printer, printDlg.GetDevMode(), &m_printJobInfo, 0, 0);
		prnPreWnd->SetPage(0);
		XASSERT(prnPreWnd->Create(*this, rcDefault, L"Print Preview", WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE));
		prnPreWnd->ShowWindow(SW_SHOWNORMAL);
	}
	
	//prnPreWnd.SetEnhMetaFile(hEmf);
	
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
			CheckMenuItem(hDebugMenu, ID_DEBUG_DRAWVERTICES, App()->GetSettings()->debugDrawVertices ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHFILL,App()->GetSettings()->debugDisableFillPath ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHSUBDIVISION, App()->GetSettings()->debugDisableSubdiv ? MF_CHECKED : MF_UNCHECKED);
		}
	}
}

// ---------------------------------------------------------------------------
