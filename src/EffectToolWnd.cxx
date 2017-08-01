/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
#include "stdafx.h"
#include "CartelManiaApp.h"
#include "EffectToolWnd.h"
#include "bannerline.h"

HWND EffectToolWnd::Create(_In_ HWND hWndParent, _In_ RECT rcInitial)
{
	return CWindowImpl::Create(hWndParent, rcInitial, L"Effect", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, WS_EX_PALETTEWINDOW);
}

void EffectToolWnd::CreateControls()
{
	const auto effectTable = App()->GetEffectTable();
	m_listbox.Create(m_hWnd, rcDefault, 0, WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | WS_VISIBLE);

	for (int i = 0; i < effectTable->GetCount(); ++i)
	{
		m_listbox.AddString(effectTable->NameAt(i).c_str());
	}

	m_listbox.SetFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	m_listbox.SetCurSel(0);
}

void EffectToolWnd::UpdateUI()
{
	auto fxName = App()->GetMainWindow()->GetBannerLineFromSelState()->GetTextFxName();
	m_listbox.SetCurSel(m_listbox.FindString(0, fxName.c_str()));
}

void EffectToolWnd::OnClose()
{
	App()->GetMainWindow()->NotifyToolboxClose(this->m_hWnd);
	DefWindowProc();
}

LRESULT EffectToolWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (HIWORD(wParam) == LBN_SELCHANGE)
	{
		int iSel = m_listbox.GetCurSel();
		if (iSel != LB_ERR)
		{
			auto bannerLine = App()->GetMainWindow()->GetBannerLineFromSelState();
			auto textBuffer = std::make_unique<wchar_t[]>(m_listbox.GetTextLen(iSel) + 1);
			m_listbox.GetText(iSel, textBuffer.get());
			bannerLine->SetTextFx(textBuffer.get());
			App()->GetBanner()->Redraw();
			App()->GetMainWindow()->NotifyActiveToolboxes(NOTIFY_ALL & ~NOTIFY_EFFECT);
		}
	} 
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
	m_listbox.SetWindowPos(NULL, 0, 0, lpwp->cx, lpwp->cy, SWP_NOMOVE);
}
