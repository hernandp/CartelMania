#include "stdafx.h"
#include "LayoutSetupToolWnd.h"
#include "CartelManiaApp.h"

BOOL LayoutSetupToolWnd::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	auto banner = App()->GetBanner();

	m_spinPageCountX.Attach(GetDlgItem(IDC_SPIN_PAGECOUNT_X));
	m_spinPageCountY.Attach(GetDlgItem(IDC_SPIN_PAGECOUNT_Y));
	m_spinEasyGlueSize.Attach(GetDlgItem(IDC_SPIN_EASYGLUESIZE));
	m_horzFillTrackbar.Attach(GetDlgItem(IDC_HORZFILL));
	m_vertFillTrackbar.Attach(GetDlgItem(IDC_VERTFILL));

	m_spinPageCountX.SetRange(1, 60);
	m_spinPageCountY.SetRange(1, 60);
	m_spinPageCountX.SetBuddy(GetDlgItem(IDC_EDIT_PAGECOUNT_X));
	m_spinPageCountY.SetBuddy(GetDlgItem(IDC_EDIT_PAGECOUNT_Y));
	m_spinPageCountX.SetPos(banner->GetPageCountXAxis());
	m_spinPageCountY.SetPos(banner->GetPageCountYAxis());

	m_spinEasyGlueSize.SetRange(5, 150);
	m_spinEasyGlueSize.SetBuddy(GetDlgItem(IDC_EDIT_EASYGLUESIZE));
	m_spinEasyGlueSize.SetPos(banner->GetEasyGlueMarginSizeMm());

	m_chkEasyGlueVisible.Attach(GetDlgItem(IDC_CHECK_EASYGLUEVISIBLE));
	m_chkEnableEasyGlue.Attach(GetDlgItem(IDC_CHECK_EASYGLUE));
	m_chkEasyGlueVisible.SetCheck(banner->IsEasyGlueActive());
	m_chkEnableEasyGlue.SetCheck(banner->IsEasyGlueMarginVisible());

	m_horzFillTrackbar.SetRange(10, 100);
	m_vertFillTrackbar.SetRange(10, 100);
	m_horzFillTrackbar.SetPos(banner->GetHorizontalFill());
	m_vertFillTrackbar.SetPos(banner->GetVerticalFill());

	SetDlgItemText(IDC_UNITS, App()->GetMeasureStringShort().c_str());
	
	wchar_t hFillText[10], vFillText[10];
	StringCchPrintf(hFillText, 10, L"%d%%", banner->GetHorizontalFill());
	StringCchPrintf(vFillText, 10, L"%d%%", banner->GetVerticalFill());
	SetDlgItemText(IDC_HORZFILL_TEXT, hFillText);
	SetDlgItemText(IDC_VERTFILL_TEXT, vFillText);
	SetupAlignComboCtls();
	UpdateBannerSizeUI();
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

void LayoutSetupToolWnd::UpdateBannerSizeUI()
{
	Gdiplus::Size bannerSizeMm = App()->GetBanner()->GetSizeMm();

	wchar_t maxString[255];
	StringCchPrintf(maxString, _countof(maxString), L"%d %s x %d %s", bannerSizeMm.Width,
		App()->GetMeasureStringShort().c_str(), bannerSizeMm.Height, App()->GetMeasureStringShort().c_str());
	SetDlgItemText(IDC_BANNERSIZE_TEXT, maxString);
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
			App()->GetBanner()->SetHorizontalFill(m_horzFillTrackbar.GetPos());
			App()->GetBanner()->SetVerticalFill(m_vertFillTrackbar.GetPos());
			App()->GetBanner()->Redraw();
		}
		break;
	}
}

void LayoutSetupToolWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
{
	switch (pScrollBar.GetDlgCtrlID())
	{
		case IDC_SPIN_PAGECOUNT_X:
		case IDC_SPIN_PAGECOUNT_Y:
			App()->GetBanner()->SetPageCount(m_spinPageCountX.GetPos(), m_spinPageCountY.GetPos());
			break;

		case IDC_SPIN_EASYGLUESIZE:
			App()->GetBanner()->SetEasyGlueMarginSizeMm(m_spinEasyGlueSize.GetPos());
			break;
	}

	UpdateBannerSizeUI();
	App()->GetMainWindow()->SetPageAreaDADirty();
	App()->GetBanner()->Redraw();
}

LRESULT LayoutSetupToolWnd::OnVAlignChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{	
	CComboBox cboVAlign;
	cboVAlign.Attach(GetDlgItem(IDC_VALIGN));
	App()->GetBanner()->SetVerticalAlignment(static_cast<BannerVerticalAlignment>(cboVAlign.GetItemData(cboVAlign.GetCurSel())));
	App()->GetBanner()->Redraw();
	return 0L;
}

LRESULT LayoutSetupToolWnd::OnHAlignChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CComboBox cboHAlign;
	cboHAlign.Attach(GetDlgItem(IDC_HALIGN));
	App()->GetBanner()->SetHorizontalAlignment(static_cast<BannerHorizontalAlignment>(cboHAlign.GetItemData(cboHAlign.GetCurSel())));
	App()->GetBanner()->Redraw();
	return 0L;
}

LRESULT LayoutSetupToolWnd::OnCheckEasyGlueClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	BOOL f = m_chkEnableEasyGlue.GetCheck();
	App()->GetBanner()->SetEasyGlueActive(f ? true : false);
	m_chkEasyGlueVisible.EnableWindow(f);
	m_spinEasyGlueSize.EnableWindow(f);
	GetDlgItem(IDC_EDIT_EASYGLUESIZE).EnableWindow(f);
	UpdateBannerSizeUI();
	App()->GetMainWindow()->SetPageAreaDADirty();
	App()->GetBanner()->Redraw();
	return 0L;
}

LRESULT LayoutSetupToolWnd::OnCheckEasyGlueVisibleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	BOOL f = m_chkEasyGlueVisible.GetCheck();
	App()->GetBanner()->SetEasyGlueMarginVisible(f ? true : false);
	return 0L;
}

LRESULT LayoutSetupToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return DefWindowProc();
}
