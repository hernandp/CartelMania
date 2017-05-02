#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include "resource.h"
#include "banner.h"

class TextEditDialog : public CDialogImpl<TextEditDialog>
{
public:
	const int IDD = IDD_TEXTEDIT;

	TextEditDialog(Banner* banner) : m_banner(banner) {}

private:

	BEGIN_MSG_MAP(TextEditDialog)
		MSG_WM_INITDIALOG(OnInitDialog);
	MSG_WM_CLOSE(OnClose);
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnClose();

	Banner*	m_banner;
	CEdit	m_edit1;
	CEdit	m_edit2;
};