#include "stdafx.h"
#include "AboutDlg.h"

BOOL AboutDialog::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	const wchar_t* szLicense = L"This program is free software: you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by "
		"the Free Software Foundation, either version 3 of the License, or "
		"(at your option) any later version.\n"
		"This program is distributed in the hope that it will be useful,"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the "
		"GNU General Public License for more details.\n"
		"You should have received a copy of the GNU General Public License "
		"along with this program.If not, see <http://www.gnu.org/licenses/>.";

	CStatic sLicenseTerms(GetDlgItem(IDC_LICENSETERMS));
	CStatic sVersion(GetDlgItem(IDC_VERSION));

	CClientDC dc(this->m_hWnd);
	
	HFONT hFontTitle = m_fontTitle.CreatePointFont(180, L"Arial", dc, true);
	HFONT hFontVersion = m_fontVersion.CreatePointFont(100, L"Arial", dc);
	HFONT hFontLicense = m_fontLicense.CreatePointFont(80, L"Arial", dc);

	sLicenseTerms.SetFont(hFontLicense);
	sLicenseTerms.SetWindowTextW(szLicense);

	sVersion.SetFont(hFontVersion);

	return TRUE;
}

LRESULT AboutDialog::OnOkClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	return  EndDialog(0);
}
