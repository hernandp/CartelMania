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

void LineLayoutToolWnd::OnClose()
{
	App()->GetMainWindow()->NotifyToolboxClose(this->m_hWnd);
	DefWindowProc();
}

void LineLayoutToolWnd::OnWindowPosChanged(LPWINDOWPOS lpwp)
{
	App()->GetSettings()->lastLineLayoutToolPos.x = lpwp->x;
	App()->GetSettings()->lastLineLayoutToolPos.y = lpwp->y;
	App()->GetSettings()->lastLineLayoutToolSize.x = lpwp->cx;
	App()->GetSettings()->lastLineLayoutToolSize.y = lpwp->cy;
	lb.SetWindowPos(NULL, 0, 0, lpwp->cx, lpwp->cy, SWP_NOMOVE);
}

LRESULT LineLayoutToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return ::DefWindowProc(m_hWnd, WM_NCACTIVATE, TRUE, lParam);
}
