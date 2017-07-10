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
	m_chkBold.Attach(GetDlgItem(IDC_CHKBOLD));
	m_fontCombo.Attach(GetDlgItem(IDC_FONTLIST));

	LOGFONT logFont = { 0 };
	logFont.lfCharSet = DEFAULT_CHARSET;

	EnumFontFamiliesEx(*App()->GetPrinterDC(), &logFont, EnumFontFamExProc, reinterpret_cast<LPARAM>(this), 0);
	
	UpdateUI();
	m_bInitialized = true;
	return TRUE;
}


int TextEditToolWnd::EnumFontFamExProc(const LOGFONT * lpelfe, const TEXTMETRIC * lpntme, DWORD FontType, LPARAM lParam)
{
	static set<wstring> s_fontNames;
	TextEditToolWnd* pInstance = (TextEditToolWnd*) lParam;
	
	if (s_fontNames.find(lpelfe->lfFaceName) == s_fontNames.end())
	{
		s_fontNames.insert(lpelfe->lfFaceName);
		pInstance->m_fontCombo.AddString(lpelfe->lfFaceName);
	}
	return 1;
}

void TextEditToolWnd::OnWindowPosChanged(LPWINDOWPOS lpwp)
{
	App()->GetSettings()->lastTextEditToolPos.x = lpwp->x;
	App()->GetSettings()->lastTextEditToolPos.y = lpwp->y;
}

void TextEditToolWnd::OnClose()
{
	App()->GetMainWindow()->NotifyToolboxClose(m_hWnd);
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

LRESULT TextEditToolWnd::OnFontSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	int iSel = m_fontCombo.GetCurSel();
	if (iSel != -1)
	{
		auto bannerLine = App()->GetMainWindow()->GetBannerLineFromSelState();
		auto textBuffer = make_unique<wchar_t[]>(m_fontCombo.GetLBTextLen(iSel) + sizeof(wchar_t));
		m_fontCombo.GetLBText(m_fontCombo.GetCurSel(), textBuffer.get());
		bannerLine->SetFontName(textBuffer.get());
		bannerLine->InvalidatePath();
		App()->GetBanner()->Redraw();
	}

	return 0;
}

LRESULT TextEditToolWnd::OnBoldClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	auto bannerLine = App()->GetMainWindow()->GetBannerLineFromSelState();
	bannerLine->SetFontStyleBold(m_chkBold.GetCheck() ? true : false);
	bannerLine->InvalidatePath();
	App()->GetBanner()->Redraw();
	return 0L;
}

void TextEditToolWnd::UpdateUI()
{
	auto bannerLine = App()->GetMainWindow()->GetBannerLineFromSelState();
	m_edit1.SetWindowTextW(bannerLine->GetText().c_str());	
	m_chkBold.SetCheck(bannerLine->IsFontStyleBold());
	m_fontCombo.SetCurSel(m_fontCombo.FindStringExact(0, bannerLine->GetFontName().c_str()));
}

LRESULT TextEditToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return ::DefWindowProc(m_hWnd, WM_NCACTIVATE, TRUE, lParam);
}
