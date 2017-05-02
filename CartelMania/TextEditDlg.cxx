#include "stdafx.h"
#include "TextEditDlg.h"
#include "banner.h"

using namespace std;

BOOL TextEditDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	//m_banner->GetTopLine();
	return TRUE;
}

void TextEditDialog::OnClose()
{
	ShowWindow(SW_HIDE);
}
