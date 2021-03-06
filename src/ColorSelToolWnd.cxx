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
#include "stdafx.h"
#include "ColorSelToolWnd.h"
#include "ColorComboBox.h"
#include "Debug.h"
#include "colorTable.h"
#include "banner.h"
#include "bannerline.h"
#include "CartelManiaApp.h"

using namespace std;

const int MarginLeft = 16;
const int MarginRight = 16;
const int ControlVSpacing = 6;
const int ControlHSpacing = 8;
const int LabelWidth = 60;
const int ComboWidth = 180;
const int WindowMarginTop = 16;
const int WindowMarginBottom = 16;
int ComboHeight;


BOOL ColorSelectToolWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	DefWindowProc();
	SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG) COLOR_BTNFACE + 1);
	CreateControls();
	return TRUE;
}

void ColorSelectToolWnd::CreateControls()
{
	auto textrend = App()->GetMainWindow()->GetBannerLineFromSelState()->GetTextFx();

	const int colPropCount = textrend->GetColorPropertyCount();
	
	for (int i = 0; i < colPropCount; ++i)
	{
		CStatic label;
		auto colorCombo = make_unique<CmColorComboBox>(App()->GetColorTable());

		HWND hLabel = label.Create(*this, rcDefault, L"", WS_CHILD);
		HWND hCombo = colorCombo->Create(*this, rcDefault, WS_CHILD);
		XASSERT(hLabel != nullptr && hCombo != nullptr);

		label.SetWindowTextW(textrend->GetColorPropertyItem(i).GetClassString());
		label.SetFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));

		RECT rc;
		colorCombo->GetWindowRect(&rc);
		ComboHeight = rc.bottom - rc.top;

		label.SetWindowPos(NULL,
			MarginLeft,
			((ControlVSpacing + ComboHeight) * i) + WindowMarginTop,
			LabelWidth,
			ComboHeight,
			SWP_NOZORDER | SWP_SHOWWINDOW);

		colorCombo->SetWindowPos(NULL,
			MarginLeft + LabelWidth + ControlHSpacing,
			((ControlVSpacing + ComboHeight) * i) + WindowMarginTop,
			ComboWidth,
			6 * ComboHeight, // drop size
			SWP_NOZORDER | SWP_SHOWWINDOW);

		colorCombo->SetTag(static_cast<int>(textrend->GetColorPropertyItem(i).GetClass()));

		colorCombo->SetCurSelByColorName(textrend->GetColorPropertyItem(i).GetValue());

		m_labelCtlList.push_back(label);
		m_comboCtlList.push_back(move(colorCombo));
	}
		
	// Adjust window to contents

	int cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	int iTotalHeight = (colPropCount * (ControlVSpacing + ComboHeight) + WindowMarginTop);

	RECT rcDesiredClient{ 0, 0, MarginLeft + LabelWidth + ControlHSpacing + ComboWidth + cxVScroll,
		iTotalHeight + WindowMarginBottom };

	AdjustWindowRectEx(&rcDesiredClient, this->GetStyle(), FALSE, this->GetExStyle());

	SetWindowPos(NULL, 0, 0, rcDesiredClient.right - rcDesiredClient.left,
		rcDesiredClient.bottom - rcDesiredClient.top, SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE);
}

HWND ColorSelectToolWnd::Create(_In_ HWND hWndParent)
{
	return CWindowImpl::Create(hWndParent, rcDefault, L"Color Selection", WS_POPUP | WS_CAPTION | WS_SYSMENU, WS_EX_PALETTEWINDOW);
}

void ColorSelectToolWnd::UpdateUI()
{
	LockWindowUpdate(TRUE);
	for (auto& ctl : m_comboCtlList)
		ctl->DestroyWindow();

	for (auto& ctl : m_labelCtlList)
		ctl.DestroyWindow();

	m_comboCtlList.clear();
	m_labelCtlList.clear();

	CreateControls();
	LockWindowUpdate(FALSE);

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);
}

LRESULT ColorSelectToolWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (HIWORD(wParam) == CBN_SELCHANGE)
	{
		for (auto& comboBox : m_comboCtlList)
		{
			ColorPropertyClass colorPropClass = (ColorPropertyClass) comboBox->GetTag().intVal;
			wstring colorValue = comboBox->GetCurSelColorName();

			auto textrend = App()->GetMainWindow()->GetBannerLineFromSelState()->GetTextFx();
			textrend->SetColorPropertyValue(colorPropClass, colorValue);
		}

		App()->GetBanner()->RegenPathAndRedraw();
	}

	return 0L;
}

LRESULT ColorSelectToolWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return DefWindowProc(WM_NCACTIVATE, TRUE, lParam);
}

void ColorSelectToolWnd::OnClose()
{
	App()->GetMainWindow()->NotifyToolboxClose(this->m_hWnd);
	DefWindowProc();
}

void ColorSelectToolWnd::OnWindowPosChanged(LPWINDOWPOS lpwPos)
{
	App()->GetSettings()->lastColorEditToolPos.x = lpwPos->x;
	App()->GetSettings()->lastColorEditToolPos.y = lpwPos->y;
}
