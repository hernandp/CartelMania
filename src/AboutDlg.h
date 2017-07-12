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

class AboutDialog : public CDialogImpl<AboutDialog>
{
public:

	const int IDD = IDD_ABOUT;

private:
	BEGIN_MSG_MAP(AboutDialog)
		MSG_WM_INITDIALOG(OnInitDialog);
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnOkClicked);
	END_MSG_MAP()

	BOOL	OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	LRESULT OnOkClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	CFont	m_fontTitle;
	CFont	m_fontVersion;
	CFont	m_fontLicense;
};