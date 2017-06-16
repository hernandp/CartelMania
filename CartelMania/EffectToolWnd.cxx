#include "stdafx.h"
#include "CartelManiaApp.h"
#include "EffectToolWnd.h"

HWND EffectToolWnd::Create(HWND hWndParent, RECT rcInitial)
{
	return CWindowImpl::Create(hWndParent, rcInitial, L"Effect", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, WS_EX_PALETTEWINDOW);
}

void EffectToolWnd::CreateControls()
{
	const auto effectTable = App()->GetEffectTable();
	lb.Create(m_hWnd, rcDefault, 0, WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | WS_VISIBLE);

	for (int i = 0; i < effectTable->GetCount(); ++i)
	{
		lb.AddString(effectTable->NameAt(i).c_str());
	}

	lb.SetFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	lb.SetCurSel(0);
}

void EffectToolWnd::UpdateUI()
{
}

void EffectToolWnd::OnClose()
{
	App()->GetMainWindow()->NotifyToolboxClose(this->m_hWnd);
	DefWindowProc();
}

LRESULT EffectToolWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	/*if (HIWORD(wParam) == LBN_SELCHANGE)
	{
		int iSel = lb.GetCurSel();
		if (iSel != LB_ERR)
		{
			auto textBuffer = std::make_unique<wchar_t[]>(lb.GetTextLen(iSel) + 1);
			lb.GetText(iSel, textBuffer.get());
			App()->GetBanner()->SetShapeName(textBuffer.get());
			App()->GetBanner()->RegenPathAndRedraw();
		}
	}*/
	return 0L;
}

int EffectToolWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	DefWindowProc();
	SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG) COLOR_BTNFACE + 1);
	CreateControls();
	return TRUE;
}

LRESULT EffectToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return DefWindowProc(WM_NCACTIVATE, TRUE, lParam);
}

void EffectToolWnd::OnWindowPosChanged(LPWINDOWPOS lpwp)
{
	App()->GetSettings()->lastShapeEditToolPos.x = lpwp->x;
	App()->GetSettings()->lastShapeEditToolPos.y = lpwp->y;
	App()->GetSettings()->lastShapeEditToolSize.x = lpwp->cx;
	App()->GetSettings()->lastShapeEditToolSize.y = lpwp->cy;
	lb.SetWindowPos(NULL, 0, 0, lpwp->cx, lpwp->cy, SWP_NOMOVE);
}
