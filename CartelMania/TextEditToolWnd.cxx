#include "stdafx.h"
#include "TextEditToolWnd.h"
#include "banner.h"
#include "CartelManiaApp.h"
#include "BannerLine.h"

using namespace std;

BOOL TextEditToolWnd::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	m_edit1.Attach(GetDlgItem(IDC_EDIT1));
	m_edit1.SetLimitText(App()->GetSettings()->maxTextLength);
	UpdateUI();
	return TRUE;
}

void TextEditToolWnd::OnWindowPosChanged(LPWINDOWPOS lpwp)
{
	App()->GetSettings()->lastTextEditToolPos.x = lpwp->x;
	App()->GetSettings()->lastTextEditToolPos.y = lpwp->y;
}

void TextEditToolWnd::OnClose()
{
	ShowWindow(SW_HIDE);
}

LRESULT TextEditToolWnd::OnEdit1Change(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	// TODO: Handle leading and trailing whitespaces!
	//
	if (m_bInitialized)
	{
		CComBSTR text;
		m_edit1.GetWindowTextW(&text);
		auto currentLine = App()->GetMainWindow()->GetBannerLineFromSelState();
				
		currentLine->SetText( (text.Length() > 0) ? std::wstring(text) : currentLine->GetDefaultText());
		App()->GetBanner()->RegenPathAndRedraw();
	}
	return 0L;
}

void TextEditToolWnd::UpdateUI()
{
	m_edit1.SetWindowTextW(App()->GetMainWindow()->GetBannerLineFromSelState()->GetText().c_str());
}

LRESULT TextEditToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return ::DefWindowProc(m_hWnd, WM_NCACTIVATE, TRUE, lParam);
}
