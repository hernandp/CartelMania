#include "stdafx.h"
#include "ColorSelToolWnd.h"
#include "ColorComboBox.h"
#include "Debug.h"
#include "colors.h"
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
	TextFx* textrend = CmApp()->GetBanner()->GetTopLine()->GetTextFx();

	const int colPropCount = textrend->GetColorPropertyCount();
	
	for (int i = 0; i < colPropCount; ++i)
	{
		CStatic label;
		auto colorCombo = make_unique<CmColorComboBox>(&g_colorTable);

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
		rcDesiredClient.bottom - rcDesiredClient.top, SWP_NOREPOSITION | SWP_NOZORDER);
}

HWND ColorSelectToolWnd::Create(HWND hWndParent)
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

			CmApp()->GetBanner()->GetTopLine()->GetTextFx()->SetColorPropertyValue(colorPropClass, colorValue);
		}

		CmApp()->GetBanner()->Invalidate();
	}

	return 0L;
}
