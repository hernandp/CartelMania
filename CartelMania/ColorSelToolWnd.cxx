#include "stdafx.h"
#include "ColorSelToolWnd.h"
#include "ColorComboBox.h"
#include "Debug.h"
#include "colors.h"

using namespace std;

BOOL ColorSelectToolWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	DefWindowProc();
	SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG) COLOR_BTNFACE + 1);

	const int MarginLeft = 16;
	const int MarginRight = 16;
	const int ControlVSpacing = 6;
	const int ControlHSpacing = 8;
	const int LabelWidth = 60;
	const int ComboWidth = 180;
	const int WindowMarginTop = 16;
	const int WindowMarginBottom = 16;
	const int ControlCount = 4;
	int ComboHeight;

	for (int i = 0; i < ControlCount; i++)
	{
		CStatic label;
		auto colorCombo = make_unique<CmColorComboBox>(g_bmColors, g_bmColorsCount);

		HWND hLabel = label.Create(*this, rcDefault, L"", WS_CHILD);
		HWND hCombo = colorCombo->Create(*this, rcDefault, WS_CHILD);
		XASSERT(hLabel != nullptr && hCombo != nullptr);

		label.SetWindowTextW(g_bmColors[i].GetName().c_str());
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

		m_label.push_back(label);
		m_combo.push_back(move(colorCombo));
	}

	// Adjust window to contents

	int cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	int iTotalHeight = (ControlCount * (ControlVSpacing + ComboHeight) + WindowMarginTop);

	RECT rcDesiredClient{ 0, 0, MarginLeft + LabelWidth + ControlHSpacing + ComboWidth + cxVScroll,
		iTotalHeight + WindowMarginBottom};

	AdjustWindowRectEx(&rcDesiredClient, this->GetStyle(), FALSE, this->GetExStyle());

	SetWindowPos(NULL, 0, 0, rcDesiredClient.right - rcDesiredClient.left,
		rcDesiredClient.bottom - rcDesiredClient.top, SWP_NOREPOSITION | SWP_NOZORDER);

	return TRUE;
}

HWND ColorSelectToolWnd::Create(HWND hWndParent)
{
	return CWindowImpl::Create(hWndParent, rcDefault, L"Color Selection", WS_POPUP | WS_CAPTION, WS_EX_PALETTEWINDOW);
}
