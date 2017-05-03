#include "stdafx.h"
#include "TextEditToolWnd.h"
#include "banner.h"
#include "CartelManiaApp.h"
#include "BannerLine.h"

using namespace std;

BOOL TextEditToolWnd::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	m_edit1.Attach(GetDlgItem(IDC_EDIT1));
	m_edit2.Attach(GetDlgItem(IDC_EDIT2));
	m_edit1.SetLimitText(CmApp()->GetSettings()->maxTextLength);
	m_edit2.SetLimitText(CmApp()->GetSettings()->maxTextLength);
	m_edit1.SetWindowTextW(CmApp()->GetBanner()->GetTopLine()->GetText().c_str());
	m_edit2.SetWindowTextW(CmApp()->GetBanner()->GetBottomLine()->GetText().c_str());
	LayoutUpdate(CmApp()->GetBanner()->GetLayout());
	m_bInitialized = true;
	return TRUE;
}

void TextEditToolWnd::OnMove(CPoint pos)
{
	CmApp()->GetSettings()->lastTextEditToolPos.x = pos.x;
	CmApp()->GetSettings()->lastTextEditToolPos.y = pos.y;
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
				
		CmApp()->GetBanner()->GetTopLine()->SetText(text.Length() > 0 ? std::wstring(text) : L"Line 1");
		CmApp()->GetBanner()->Invalidate();
	}
	return 0L;
}

LRESULT TextEditToolWnd::OnEdit2Change(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	// TODO: Handle leading and trailing whitespaces!
	//
	if (m_bInitialized)
	{
		CComBSTR text;
		m_edit2.GetWindowTextW(&text);
		CmApp()->GetBanner()->GetBottomLine()->SetText(text.Length() > 0 ? std::wstring(text) : L"Line 2");
		CmApp()->GetBanner()->Invalidate();
	}
	return 0L;
}

void TextEditToolWnd::LayoutUpdate(BannerLayout newLayout)
{
	RECT rcEdit, rcWnd;
	m_edit2.GetWindowRect(&rcEdit);
	GetWindowRect(&rcWnd);

	if (newLayout == BannerLayout::SingleLine)
	{		
		SetWindowPos(nullptr, -1, -1, rcWnd.right - rcWnd.left,
			(rcWnd.bottom - rcWnd.top) - (rcEdit.bottom - rcEdit.top), SWP_NOZORDER | SWP_NOMOVE);

		m_edit2.ShowWindow(SW_HIDE);
	}
	else
	{
		SetWindowPos(nullptr, -1, -1, rcWnd.right - rcWnd.left,
			(rcWnd.bottom - rcWnd.top) + (rcEdit.bottom - rcEdit.top), SWP_NOZORDER | SWP_NOMOVE);

		m_edit2.ShowWindow(SW_SHOW);
	}
}
