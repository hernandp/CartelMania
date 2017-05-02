#include "stdafx.h"
#include "TextEditDlg.h"
#include "banner.h"
#include "CartelManiaApp.h"

using namespace std;

BOOL TextEditDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	m_edit1.Attach(GetDlgItem(IDC_EDIT1));
	m_edit2.Attach(GetDlgItem(IDC_EDIT2));
	m_edit1.SetLimitText(CmApp()->GetGlobalSettings()->maxTextLength);
	m_edit2.SetLimitText(CmApp()->GetGlobalSettings()->maxTextLength);
	return TRUE;
}

void TextEditDialog::OnMove(CPoint pos)
{
	CmApp()->GetGlobalSettings()->lastTextEditToolPos.x = pos.x;
	CmApp()->GetGlobalSettings()->lastTextEditToolPos.y = pos.y;
}

void TextEditDialog::OnClose()
{
	ShowWindow(SW_HIDE);
}
