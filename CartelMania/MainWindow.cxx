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
#include "Geometry.h"

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
	XASSERT(m_statusBar.Create(*this, rcDefault, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, NULL, IDC_STATUSBAR));
	XASSERT(m_imgList.Create(32,32, ILC_COLOR32 | ILC_MASK, 10, 10));
	XASSERT(m_toolbar.Create(*this, 0, nullptr, WS_VISIBLE | WS_CHILD , TBSTYLE_EX_DOUBLEBUFFER, IDC_TOOLBAR));
	
	auto hMod = GetModuleHandle(0);
	m_imgList.AddIcon(LoadIcon(hMod, MAKEINTRESOURCE(IDI_COLORTOOLBOX)));
	m_imgList.AddIcon(LoadIcon(hMod, MAKEINTRESOURCE(IDI_TEXTEDIT)));
	m_imgList.AddIcon(LoadIcon(hMod, MAKEINTRESOURCE(IDI_LINELAYOUT)));
	m_imgList.AddIcon(LoadIcon(hMod, MAKEINTRESOURCE(IDI_PAGELAYOUT)));
	m_imgList.AddIcon(LoadIcon(hMod, MAKEINTRESOURCE(IDI_SHAPETOOL)));
	m_toolbar.SetImageList(m_imgList, 0);
	m_toolbar.LoadStdImages(IDB_STD_LARGE_COLOR);

	const DWORD buttonStyles = BTNS_AUTOSIZE;
	TBBUTTON tbButtons[] =
	{
		{ 0, ID_CMD_COLORTOOL,		TBSTATE_ENABLED, buttonStyles | BTNS_CHECK, {0}, 0, (INT_PTR) L"Color" },
		{ 1, ID_CMD_EDITTEXT,		TBSTATE_ENABLED, buttonStyles | BTNS_CHECK, {0}, 0, (INT_PTR) L"Text" },
		{ 2, ID_CMD_LINELAYOUTTOOL,	TBSTATE_ENABLED, buttonStyles | BTNS_CHECK, {0}, 0, (INT_PTR) L"Line Layout" },
		{ 4, ID_CMD_OPENSHAPETOOL,	TBSTATE_ENABLED, buttonStyles | BTNS_CHECK, {0}, 0, (INT_PTR) L"Shape" },
		{ 3, ID_CMD_LAYOUTSETUPTOOL,TBSTATE_ENABLED, buttonStyles | BTNS_CHECK, {0}, 0, (INT_PTR) L"Page Layout" },
		{ MAKELONG(0       ,      0), NULL, NULL, TBSTYLE_SEP, {0}, 0, 0 },
		{ MAKELONG(STD_PRINTPRE,  0), ID_CMD_PRINTPRE, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 },
		{ MAKELONG(STD_PRINT,     0), ID_CMD_PRINT, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 },
		{ MAKELONG(STD_PRINT,     0), ID_CMD_PAGESETUP, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0 }
	};
	m_toolbar.SetButtonStructSize(sizeof(TBBUTTON));
	m_toolbar.AddButtons(_countof(tbButtons), tbButtons);
	m_toolbar.AutoSize();

	UpdateMenu();
	return 1L;
}

LRESULT CManiaMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	m_pageDADirty = true;
	m_toolbar.SendMessageW(WM_SIZE, wParam, lParam);
	m_statusBar.SendMessageW(WM_SIZE, wParam, lParam);
	return 0L;
}

LRESULT CManiaMainWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	auto banner = App()->GetBanner();

	CRect rcPageDA;
	Gdiplus::RectF line1Rect, line2Rect;
	GetPageDisplayAreaRect(&rcPageDA);
	banner->GetLineRects(banner->CalcRect(&rcPageDA), line1Rect, line2Rect, true);

	if (line1Rect.Contains((float)pt.x, (float)pt.y))
	{
		m_lineSelState.first = true;
		m_lineSelState.second = false;
		InvalidatePageDA();
		NotifyActiveToolboxes();
	}
	else if (banner->GetLayout() != BannerLayout::SingleLine && line2Rect.Contains((float)pt.x, (float)pt.y))
	{
		m_lineSelState.first = false;
		m_lineSelState.second = true;
		InvalidatePageDA();
		NotifyActiveToolboxes();
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	// The window to be activated is one of our toolboxes? Do not show us as inactive,
	// as it's common with floating toolboxes behavior in Windows applications
	//

	HWND hWndOther = reinterpret_cast<HWND>(lParam);

	if (hWndOther && 
		(hWndOther == m_shapeSelectToolWnd.m_hWnd ||
		 hWndOther == m_textEditToolWnd.m_hWnd ||
		 hWndOther == m_colorSelectToolWnd.m_hWnd ||
		 hWndOther == m_lineLayoutToolWnd.m_hWnd ||
		 hWndOther == m_layoutSetupToolWnd.m_hWnd ) )
	{
		return DefWindowProc(WM_NCACTIVATE, TRUE, lParam);
	}

	return DefWindowProc(WM_NCACTIVATE, wParam, lParam);
}

void CManiaMainWnd::NotifyActiveToolboxes()
{
	if (m_colorSelectToolWnd)
		m_colorSelectToolWnd.UpdateUI();

	if (m_textEditToolWnd)
		m_textEditToolWnd.UpdateUI();
}

void CManiaMainWnd::InvalidatePageDA()
{
	RECT rc;
	GetPageDisplayAreaRect(&rc);
	InvalidateRect(&rc, FALSE);
}

int CManiaMainWnd::GetClientRect(_Out_ LPRECT lpRect) const
{
	int info[] = { 1, 0,
		1, IDC_TOOLBAR,
		1, IDC_STATUSBAR,
		0, 0 };
	GetEffectiveClientRect(m_hWnd, lpRect, info);
	return TRUE;
}

void CManiaMainWnd::CalcPixelsPerMm(float& horizontalRatio, float& verticalRatio)
{
	CRect rc;
	GetPageDisplayAreaRect(&rc);
	horizontalRatio = float(rc.Width()) / float(App()->GetBanner()->GetSizeMm().Width);
	verticalRatio = float(rc.Height()) / float(App()->GetBanner()->GetSizeMm().Height);
}

void CManiaMainWnd::DoPaint(CDCHandle hDC)
{
	DrawClientArea(hDC);

	CRect rcPageDA;
	GetPageDisplayAreaRect(&rcPageDA);

	auto banner = App()->GetBanner();
	banner->PaintOn(hDC, &rcPageDA);

	// Draw page divisions, if needed

	Gdiplus::Graphics gr(hDC);

	Gdiplus::Pen dashPen(Gdiplus::Color::Gray, 2);
	dashPen.SetDashStyle(Gdiplus::DashStyleDot);

	auto pageCountX = banner->GetPageCountXAxis();
	auto pageCountY = banner->GetPageCountYAxis();	
	float easyGlueMarginMm = (float) banner->IsEasyGlueActive() ? banner->GetEasyGlueMarginSizeMm() : 0.0f;
	float xPixelsPerMm, yPixelsPerMm;

	CalcPixelsPerMm(xPixelsPerMm, yPixelsPerMm);
	
	int x = rcPageDA.left;
	
	for (int i = 1; i < pageCountX; ++i)
	{
		x += int((App()->GetPrintableAreaMm().Width * xPixelsPerMm) - (easyGlueMarginMm * xPixelsPerMm));
		gr.DrawLine(&dashPen, x, rcPageDA.top, x, rcPageDA.bottom);
	}

	int y = rcPageDA.top;
	for (int i = 1; i < pageCountY; ++i)
	{
		y += int((App()->GetPrintableAreaMm().Height * yPixelsPerMm) - (easyGlueMarginMm * yPixelsPerMm));
		gr.DrawLine(&dashPen, rcPageDA.left, y, rcPageDA.right, y);
	}

	// Selection marks

	Gdiplus::RectF line1Rect, line2Rect;
	Gdiplus::RectF bannerRect = banner->CalcRect(&rcPageDA);
	banner->GetLineRects(bannerRect, line1Rect, line2Rect, true);

	if (App()->GetMainWindow()->GetLineSelState().first || App()->GetBanner()->GetLayout() == BannerLayout::SingleLine)
	{
		gr.TranslateTransform(line1Rect.X, line1Rect.Y);
		DrawSelectionMark(gr, line1Rect);
		gr.TranslateTransform(-line1Rect.X, -line1Rect.Y);
	}

	if (App()->GetMainWindow()->GetLineSelState().second )
	{
		gr.TranslateTransform(line2Rect.X, line2Rect.Y);
		DrawSelectionMark(gr, line2Rect);
		gr.TranslateTransform(-line2Rect.X, -line2Rect.Y);
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

void CManiaMainWnd::DoPageSetupDialog()
{
	CPageSetupDialog psDlg(PSD_INTHOUSANDTHSOFINCHES);
	if (psDlg.DoModal() == IDOK)
	{
		psDlg.GetPaperSize();

		/*DEVMODE* pDevMode = psDlg.GetDevMode();
		if (pDevMode)
		{
			CDevMode dm(pDevMode);
			dm.m_pDevMode->dmS
		}

		psDlg.GetPaperSize();*/
	}
}

bool CManiaMainWnd::GetPageDisplayAreaRect(RECT* lpRect)
{
	if (!m_pageDADirty)
	{
		*lpRect = m_cachedPageDA;
	}
	else
	{
		const Gdiplus::Size bannerSize = App()->GetBanner()->GetSizeMm();
		Gdiplus::RectF pageRect(0, 0, float(bannerSize.Width), float(bannerSize.Height));

		const int clientAreaMarginTop = 16;
		const int clientAreaMarginBottom = 16;
		const int clientAreaMarginSides = 16;

		// Adjust the preliminary rect to ratio to fill window client area
		CRect rcClient;
		GetClientRect(&rcClient);
		const Gdiplus::RectF rcClientArea(
			float(rcClient.left + clientAreaMarginSides),
			float(rcClient.top + clientAreaMarginTop),
			float(rcClient.Width() - clientAreaMarginSides),
			float(rcClient.Height() - clientAreaMarginBottom)
		);

		const float bannerRectRatio = CalcAspectRatioToFit((float) bannerSize.Width, (float) bannerSize.Height,
			rcClientArea.Width, rcClientArea.Height);

		//const int cxRulerTop = int(14.0f * gr.GetDpiY() / 72.0f);
		pageRect.Height *= bannerRectRatio;
		pageRect.Width *= bannerRectRatio;
		pageRect.X = (rcClient.Width() / 2) - (pageRect.Width / 2);
		pageRect.Y = rcClient.top + (rcClient.Height() / 2) - (pageRect.Height / 2);

		lpRect->top = (LONG) pageRect.Y + clientAreaMarginTop;
		lpRect->left = (LONG) pageRect.X + clientAreaMarginSides;
		lpRect->bottom = LONG(pageRect.Y + pageRect.Height - clientAreaMarginBottom);
		lpRect->right = LONG(pageRect.X + pageRect.Width - clientAreaMarginSides);

		m_cachedPageDA = *lpRect;
		m_pageDADirty = false;
	}

	return true;
}

void CManiaMainWnd::DrawClientArea(CDCHandle hDC)
{
	Gdiplus::Graphics gr(hDC);
	CRect rcClient;

	GetClientRect(&rcClient);
	const Gdiplus::RectF rcClientArea((float) rcClient.left, (float) rcClient.top, float(rcClient.Width()), float(rcClient.Height()));

	// Background

	gr.FillRectangle(&Gdiplus::LinearGradientBrush(rcClientArea,
		Gdiplus::Color::DarkSlateBlue,
		Gdiplus::Color::DarkCyan,
		Gdiplus::LinearGradientModeVertical), rcClientArea);

	// Get current page size

	CRect rcPageDA;
	if (!GetPageDisplayAreaRect(&rcPageDA))
	{
		MessageBox(L"Cannot determine the current printer page size.\n\nPress Ok to select your page configuration.");
		DoPageSetupDialog();
	}

	// Draw banner WYSIWYG client area, keeping banner length and width aspect ratio.

	Gdiplus::Font rulerFont(&Gdiplus::FontFamily(L"Arial"), 14);

	// Draw page+shadow

	CRect pageShadow = rcPageDA;
	pageShadow.OffsetRect(1, 1);

	for (int i = 0; i < 16; ++i)
	{
		pageShadow.OffsetRect(16 - i, 16 - i);
		gr.DrawLine(&Gdiplus::Pen(Gdiplus::Color(i * (128 / 16), 0, 0, 0)),
			Gdiplus::Point(rcPageDA.left + rcPageDA.Width() + 16 - i, rcPageDA.top + 16 - i),
			Gdiplus::Point(rcPageDA.left + rcPageDA.Width() + 16 - i, rcPageDA.top + rcPageDA.Height() + 16 - i));
	}

	gr.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color::White),
		Gdiplus::Rect(rcPageDA.left, rcPageDA.top, rcPageDA.Width(), rcPageDA.Height()));	
}

LRESULT CManiaMainWnd::OnEditText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (m_textEditToolWnd && m_textEditToolWnd.IsWindowVisible())
	{
		m_textEditToolWnd.ShowWindow(SW_HIDE);
	}
	else
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
	}

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
			m_lineSelState.second = true;
			UpdateMenu();
			break;

		case ID_EDIT_SELECTBOTH:
			m_lineSelState.first = true;
			m_lineSelState.second = true;
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
	App()->GetBanner()->RegenPathAndRedraw();
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
			m_textEditToolWnd.UpdateUI();
		}

		App()->GetBanner()->SetLayout(selectedLayout);
		InvalidateRect(nullptr, FALSE);
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnLayoutSetupTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (m_layoutSetupToolWnd && m_layoutSetupToolWnd.IsWindowVisible())
	{
		m_layoutSetupToolWnd.ShowWindow(SW_HIDE);
	}
	else
	{
		if (!m_layoutSetupToolWnd.m_hWnd)
			XASSERT(m_layoutSetupToolWnd.Create(m_hWnd));

		auto lastX = App()->GetSettings()->lastLayoutEditToolPos.x;
		auto lastY = App()->GetSettings()->lastLayoutEditToolPos.y;

		if (lastX == -1 && lastY == -1)
		{
			// Position default 
			lastX = 0;
			lastY = 0;
		}

		m_layoutSetupToolWnd.SetWindowPos(nullptr, lastX, lastY, -1, -1, SWP_NOZORDER | SWP_NOSIZE);
		m_layoutSetupToolWnd.ShowWindow(SW_SHOWNA);
	}
	return 0L;
}

LRESULT CManiaMainWnd::OnLineLayoutTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_lineLayoutToolWnd.m_hWnd && m_lineLayoutToolWnd.IsWindowVisible())
	{
		m_lineLayoutToolWnd.ShowWindow(SW_HIDE);
	}
	else
	{
		auto lastX = App()->GetSettings()->lastLineLayoutToolPos.x;
		auto lastY = App()->GetSettings()->lastLineLayoutToolPos.y;
		auto lastSizeX = App()->GetSettings()->lastLineLayoutToolSize.x;
		auto lastSizeY = App()->GetSettings()->lastLineLayoutToolSize.y;

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

		if (m_lineLayoutToolWnd.m_hWnd == nullptr)
			XASSERT(m_lineLayoutToolWnd.Create(m_hWnd, RECT{ lastX,lastY,lastX + lastSizeX,lastY + lastSizeY }));

		m_lineLayoutToolWnd.SetWindowPos(nullptr, lastX, lastY, lastSizeX, lastSizeY, SWP_NOZORDER);
		m_lineLayoutToolWnd.ShowWindow(SW_SHOWNA);
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

BannerLine* CManiaMainWnd::GetBannerLineFromSelState()
{
	if (m_lineSelState.first)
		return App()->GetBanner()->GetTopLine();
	else if (m_lineSelState.second)
		return App()->GetBanner()->GetBottomLine();
	else
		return nullptr; // This should not happen.
}

void CManiaMainWnd::NotifyToolboxClose(HWND hWnd)
{
	if (m_colorSelectToolWnd.m_hWnd == hWnd)
		m_toolbar.CheckButton(ID_CMD_COLORTOOL, FALSE);

	if (m_layoutSetupToolWnd.m_hWnd == hWnd)
		m_toolbar.CheckButton(ID_CMD_LAYOUTSETUPTOOL, FALSE);

	if (m_textEditToolWnd.m_hWnd == hWnd)
		m_toolbar.CheckButton(ID_CMD_EDITTEXT, FALSE);

	if (m_lineLayoutToolWnd.m_hWnd == hWnd)
		m_toolbar.CheckButton(ID_CMD_LINELAYOUTTOOL, FALSE);

	if (m_shapeSelectToolWnd.m_hWnd == hWnd)
		m_toolbar.CheckButton(ID_CMD_OPENSHAPETOOL, FALSE);
}

LRESULT CManiaMainWnd::OnOpenColorTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (m_colorSelectToolWnd.m_hWnd && m_colorSelectToolWnd.IsWindowVisible())
	{
		m_colorSelectToolWnd.ShowWindow(SW_HIDE);
	}
	else
	{
		auto lastX = App()->GetSettings()->lastColorEditToolPos.x;
		auto lastY = App()->GetSettings()->lastColorEditToolPos.y;

		if (lastX == -1 && lastY == -1)
		{
			// Position defaults
			RECT rc;
			GetClientRect(&rc);
			ClientToScreen(&rc);
			lastX = rc.left;
			lastY = rc.top;
		}

		if (m_colorSelectToolWnd.m_hWnd == nullptr)
			XASSERT(m_colorSelectToolWnd.Create(m_hWnd));

		m_colorSelectToolWnd.SetWindowPos(nullptr, lastX, lastY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_colorSelectToolWnd.ShowWindow(SW_SHOWNA);
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnOpenShapeTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	if (m_shapeSelectToolWnd && m_shapeSelectToolWnd.IsWindowVisible())
	{
		m_shapeSelectToolWnd.ShowWindow(SW_HIDE);
	}
	else
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
	}

	return 0L;
}

LRESULT CManiaMainWnd::OnPrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPrintJob job;
	CPrintDialogEx printDlg;
	printDlg.m_pdex.hDevMode = App()->GetDevMode()->CopyToHDEVMODE();
	printDlg.DoModal();
	
	CPrinter printer;
	if (printer.OpenPrinterW(printDlg.GetDeviceName(), printDlg.GetDevMode()))
	{
		int pageCountX = App()->GetBanner()->GetPageCountXAxis();
		int pageCountY = App()->GetBanner()->GetPageCountYAxis();
		int totalPageCount = pageCountX * pageCountY;
		m_printJobInfo.m_pageCountX = pageCountX;
		m_printJobInfo.m_pageCountY = pageCountY;

		job.StartPrintJob(false, printer.m_hPrinter, 
			printDlg.GetDevMode(), 
			&m_printJobInfo, 
			App()->GetDocumentName().c_str(),
			0,
			totalPageCount - 1);
	}

	GlobalFree(printDlg.m_pdex.hDevMode);
	return 0L;
}

LRESULT CManiaMainWnd::OnPrintPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPrinter printer;
	if (printer.OpenDefaultPrinter(App()->GetDevMode()->m_pDevMode))
	{
		CPrintPreviewWindow* prnPreWnd = new CPrintPreviewWindow;
		int pageCountX = App()->GetBanner()->GetPageCountXAxis();
		int pageCountY = App()->GetBanner()->GetPageCountYAxis();
		int totalPageCount = pageCountX * pageCountY;
		m_printJobInfo.m_pageCountX = pageCountX;
		m_printJobInfo.m_pageCountY = pageCountY;

		prnPreWnd->SetPrintPreviewInfo(printer, App()->GetDevMode()->m_pDevMode, &m_printJobInfo, 0, totalPageCount-1);
		prnPreWnd->SetPage(1);
		XASSERT(prnPreWnd->Create(*this, rcDefault, L"Print Preview", WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE));
		prnPreWnd->ShowWindow(SW_SHOWNORMAL);
	}
	return 0L;
}

LRESULT CManiaMainWnd::OnPageSetup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CPageSetupDialog pageSetupDlg;
	pageSetupDlg.DoModal();
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
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHFILL, App()->GetSettings()->debugDisableFillPath ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHSUBDIVISION, App()->GetSettings()->debugDisableSubdiv ? MF_CHECKED : MF_UNCHECKED);
		}
	}
}

// ---------------------------------------------------------------------------
