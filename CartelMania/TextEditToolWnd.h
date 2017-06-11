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
		MSG_WM_MOVE(OnMove);
		COMMAND_HANDLER(IDC_EDIT1, EN_CHANGE, OnEdit1Change)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		MSG_WM_CLOSE(OnClose);
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnMove(CPoint pos);
	void OnClose();
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnEdit1Change(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	CEdit	m_edit1;
	bool	m_bInitialized = false;
};