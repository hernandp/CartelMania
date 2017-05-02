#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include "resource.h"
#include "banner.h"

class TextEditDialog : public CDialogImpl<TextEditDialog>
{
public:
	const int IDD = IDD_TEXTEDIT;

private:

	BEGIN_MSG_MAP(TextEditDialog)
		MSG_WM_INITDIALOG(OnInitDialog);
		MSG_WM_MOVE(OnMove);
	MSG_WM_CLOSE(OnClose);
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnMove(CPoint pos);
	void OnClose();

	CEdit	m_edit1;
	CEdit	m_edit2;
};