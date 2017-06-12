#include "stdafx.h"
#include "LineLayoutToolWnd.h"
#include "CartelManiaApp.h"

HWND LineLayoutToolWnd::Create(HWND hWndParent, RECT rcInitial)
{
	return CWindowImpl::Create(hWndParent, rcInitial, L"Line Layout", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, WS_EX_PALETTEWINDOW);
}

void LineLayoutToolWnd::CreateControls()
{
	lb.Create(m_hWnd, rcDefault, 0, WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | WS_VISIBLE);

	// Keep order of BannerLayout enumeration

	lb.AddString(L"Single Line"); // 0
	lb.AddString(L"Small Over Large 3");
	lb.AddString(L"Small Over Large 2");
	lb.AddString(L"Small Over Large 1");
	lb.AddString(L"Equal Lines");
	lb.AddString(L"Large Over Small 1");
	lb.AddString(L"Large Over Small 2");
	lb.AddString(L"Large Over Small 3");
	
	lb.SetFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	lb.SetCurSel(static_cast<int>(App()->GetBanner()->GetLayout()));
}

void LineLayoutToolWnd::UpdateUI()
{
}

LRESULT LineLayoutToolWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (HIWORD(wParam) == LBN_SELCHANGE)
	{
		int iSel = lb.GetCurSel();
		if (iSel != LB_ERR)
		{
			App()->GetBanner()->SetLayout(static_cast<BannerLayout>(iSel));
			App()->GetMainWindow()->Invalidate(FALSE);
		}
	}
	return 0L;
}

int LineLayoutToolWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	DefWindowProc();
	SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG) COLOR_BTNFACE + 1);
	CreateControls();
	return TRUE;
}

void LineLayoutToolWnd::OnMove(CPoint pt)
{
	App()->GetSettings()->lastLineLayoutToolPos.x = pt.x;
	App()->GetSettings()->lastLineLayoutToolPos.y = pt.y;
}

void LineLayoutToolWnd::OnSize(UINT type, CSize size)
{
	lb.SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOZORDER);
	App()->GetSettings()->lastLineLayoutToolSize.x = size.cx;
	App()->GetSettings()->lastLineLayoutToolSize.y = size.cy;
}

LRESULT LineLayoutToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return DefWindowProc(WM_NCACTIVATE, TRUE, lParam);
}
