#include "stdafx.h"
#include "LayoutSetupToolWnd.h"
#include "CartelManiaApp.h"

BOOL LayoutSetupToolWnd::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	auto banner = App()->GetBanner();
	m_bannerHeight.Attach(GetDlgItem(IDC_BANNERHEIGHT));
	m_bannerWidth.Attach(GetDlgItem(IDC_BANNERWIDTH));
	m_horzFillTrackbar.Attach(GetDlgItem(IDC_HORZFILL));
	m_vertFillTrackbar.Attach(GetDlgItem(IDC_VERTFILL));

	SetDlgItemInt(IDC_BANNERHEIGHT, banner->GetSizeMm().Height, FALSE);
	SetDlgItemInt(IDC_BANNERWIDTH, banner->GetSizeMm().Width, FALSE);
	m_horzFillTrackbar.SetRange(10, 100);
	m_vertFillTrackbar.SetRange(10, 100);
	m_horzFillTrackbar.SetPos(banner->GetHorizontalFill());
	m_vertFillTrackbar.SetPos(banner->GetVerticalFill());
	
	wchar_t hFillText[10], vFillText[10];
	StringCchPrintf(hFillText, 10, L"%d%%", banner->GetHorizontalFill());
	StringCchPrintf(vFillText, 10, L"%d%%", banner->GetVerticalFill());
	SetDlgItemText(IDC_HORZFILL_TEXT, hFillText);
	SetDlgItemText(IDC_VERTFILL_TEXT, vFillText);

	return TRUE;
}

void LayoutSetupToolWnd::OnMove(CPoint pos)
{
}

void LayoutSetupToolWnd::OnClose()
{
	ShowWindow(SW_HIDE);
}

void LayoutSetupToolWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
{
	switch (pScrollBar.GetDlgCtrlID())
	{
		case IDC_HORZFILL:
		case IDC_VERTFILL:
		{
			auto banner = App()->GetBanner();
			wchar_t hFillText[10], vFillText[10];
			StringCchPrintf(hFillText, 10, L"%d%%", m_horzFillTrackbar.GetPos());
			StringCchPrintf(vFillText, 10, L"%d%%", m_vertFillTrackbar.GetPos());
			SetDlgItemText(IDC_HORZFILL_TEXT, hFillText);
			SetDlgItemText(IDC_VERTFILL_TEXT, vFillText);
		}
		break;
	}
}
