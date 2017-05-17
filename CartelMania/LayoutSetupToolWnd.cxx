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
	SetupAlignComboCtls();
	UpdatePrintPageCountUI();
	return TRUE;
}

void LayoutSetupToolWnd::SetupAlignComboCtls()
{
	auto banner = App()->GetBanner();
	CComboBox cboVAlign, cboHAlign;
	cboVAlign.Attach(GetDlgItem(IDC_VALIGN));
	cboHAlign.Attach(GetDlgItem(IDC_HALIGN));

	cboVAlign.AddString(L"Top");
	cboVAlign.AddString(L"Center");
	cboVAlign.AddString(L"Bottom");
	cboVAlign.SetItemData(0, (DWORD_PTR) BannerVerticalAlignment::Top);
	cboVAlign.SetItemData(1, (DWORD_PTR) BannerVerticalAlignment::Center);
	cboVAlign.SetItemData(2, (DWORD_PTR) BannerVerticalAlignment::Bottom);

	cboHAlign.AddString(L"Left");
	cboHAlign.AddString(L"Center");
	cboHAlign.AddString(L"Right");
	cboHAlign.SetItemData(0, (DWORD_PTR) BannerHorizontalAlignment::Left);
	cboHAlign.SetItemData(1, (DWORD_PTR) BannerHorizontalAlignment::Center);
	cboHAlign.SetItemData(2, (DWORD_PTR) BannerHorizontalAlignment::Right);

	for (int i = 0; i < cboVAlign.GetCount(); i++)
	{
		if ((BannerVerticalAlignment) cboVAlign.GetItemData(i) == banner->GetVerticalAlignment())
			cboVAlign.SetCurSel(i);
	}

	for (int i = 0; i < cboHAlign.GetCount(); i++)
	{
		if ((BannerHorizontalAlignment) cboVAlign.GetItemData(i) == banner->GetHorizontalAlignment())
			cboHAlign.SetCurSel(i);
	}
}

void LayoutSetupToolWnd::UpdatePrintPageCountUI()
{

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
			wchar_t hFillText[10], vFillText[10];
			StringCchPrintf(hFillText, 10, L"%d%%", m_horzFillTrackbar.GetPos());
			StringCchPrintf(vFillText, 10, L"%d%%", m_vertFillTrackbar.GetPos());
			SetDlgItemText(IDC_HORZFILL_TEXT, hFillText);
			SetDlgItemText(IDC_VERTFILL_TEXT, vFillText);
		}
		break;
	}
}

LRESULT LayoutSetupToolWnd::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	auto banner = App()->GetBanner();

	// Validate first

	// Set and request redraw
	CComboBox cboVAlign, cboHAlign;
	cboVAlign.Attach(GetDlgItem(IDC_VALIGN));
	cboHAlign.Attach(GetDlgItem(IDC_HALIGN));

	banner->SetSizeMm(Gdiplus::Size(GetDlgItemInt(IDC_BANNERWIDTH), GetDlgItemInt(IDC_BANNERHEIGHT)));
	banner->SetVerticalAlignment(static_cast<BannerVerticalAlignment>
		(cboVAlign.GetItemData(cboVAlign.GetCurSel())));
	banner->SetHorizontalAlignment(static_cast<BannerHorizontalAlignment>
		(cboHAlign.GetItemData(cboHAlign.GetCurSel())));
	banner->SetHorizontalFill(m_horzFillTrackbar.GetPos());
	banner->SetVerticalFill(m_vertFillTrackbar.GetPos());

	banner->Redraw();

	return 0;
}
