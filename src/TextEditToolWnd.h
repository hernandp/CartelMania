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
#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include "resource.h"
#include "banner.h"

class TextEditToolWnd : public CDialogImpl<TextEditToolWnd>
{
public:
	const int IDD = IDD_TEXTEDIT;

	void UpdateUI();

private:

	BEGIN_MSG_MAP(TextEditToolWnd)
		MSG_WM_INITDIALOG(OnInitDialog);
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged);
		COMMAND_HANDLER(IDC_EDIT1, EN_CHANGE, OnEdit1Change)
		COMMAND_HANDLER(IDC_FONTLIST, CBN_SELCHANGE, OnFontSelChange)
		COMMAND_HANDLER(IDC_CHKBOLD, BN_CLICKED, OnBoldClicked)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		MSG_WM_CLOSE(OnClose);
	END_MSG_MAP()

	BOOL	OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void	OnWindowPosChanged(LPWINDOWPOS);
	void	OnClose();
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnEdit1Change(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFontSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBoldClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	static int CALLBACK EnumFontFamExProc(
		const LOGFONT    *lpelfe,
		const TEXTMETRIC *lpntme,
		DWORD      FontType,
		LPARAM     lParam
	);

	CEdit		m_edit1;
	CComboBox	m_fontCombo;
	CButton		m_chkBold;
	bool	m_bInitialized = false;
};