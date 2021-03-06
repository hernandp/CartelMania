/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hern�n Di Pietro

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
#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include "resource.h"

class LayoutSetupToolWnd : public CDialogImpl<LayoutSetupToolWnd>
{
public:
	const int IDD = IDD_LAYOUTSETUPTOOL;

private:

	BEGIN_MSG_MAP(LayoutSetupToolWnd)
		MSG_WM_INITDIALOG(OnInitDialog);
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged);
		MSG_WM_CLOSE(OnClose);
		MSG_WM_HSCROLL(OnHScroll);
		MSG_WM_VSCROLL(OnVScroll);
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate);
		COMMAND_HANDLER(IDC_VALIGN, CBN_SELCHANGE, OnVAlignChange);
		COMMAND_HANDLER(IDC_HALIGN, CBN_SELCHANGE, OnHAlignChange);
		COMMAND_HANDLER(IDC_CHECK_EASYGLUE, BN_CLICKED, OnCheckEasyGlueClick);
		COMMAND_HANDLER(IDC_CHECK_EASYGLUEVISIBLE, BN_CLICKED, OnCheckEasyGlueVisibleClick);
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void UpdateBannerSizeUI();
	void SetupAlignComboCtls();
	void OnMove(CPoint pos);
	void OnClose();
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);
	void OnWindowPosChanged(LPWINDOWPOS lpwp);
	LRESULT OnVAlignChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHAlignChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCheckEasyGlueClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCheckEasyGlueVisibleClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	
	CEdit			m_pageCountX;
	CEdit			m_pageCountY;
	CUpDownCtrl		m_spinPageCountX;
	CUpDownCtrl		m_spinPageCountY;
	CUpDownCtrl		m_spinEasyGlueSize;
	CButton			m_chkEasyGlueVisible;
	CButton			m_chkEnableEasyGlue;
	CTrackBarCtrl	m_horzFillTrackbar;
	CTrackBarCtrl	m_vertFillTrackbar;
	bool			m_bInitialized = false;
};