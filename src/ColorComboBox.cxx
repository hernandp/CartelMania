#include "stdafx.h"
#include "ColorComboBox.h"
#include "colorTable.h"
#include "debug.h"
#include "CartelManiaApp.h"

using namespace Gdiplus;

const int ITEM_LEFTMARGIN = 4;
const int ITEM_RIGHTMARGIN = 4;
const int ITEM_VMARGIN = 4;
const int ITEM_COLORSAMPLE_WIDTH = 24;
const int ITEM_COLORSAMPLE_HEIGHT = 20;

HWND CmColorComboBox::Create(HWND hWndParent, _U_RECT rect, DWORD dwStyle, UINT id)
{
	return CWindowImpl::Create(hWndParent, rect, nullptr,
		CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST | WS_VSCROLL | dwStyle, NULL, id, nullptr);
}

void CmColorComboBox::SetCurSelByColorName(const std::wstring & name)
{
	for (int i = 0; i < GetCount(); ++i)
	{
		if (!lstrcmpi(name.c_str(), (LPCWSTR) GetItemData(i)))
		{
			SetCurSel(i);
			break;
		}
	}	
}

std::wstring CmColorComboBox::GetCurSelColorName() const
{
	return std::wstring((LPCWSTR) GetItemData(GetCurSel()));
}

int CmColorComboBox::OnCreate(LPCREATESTRUCT lps)
{
	LRESULT lr = DefWindowProc();

	for (int i = 0; i < App()->GetColorTable()->GetCount(); ++i)
	{
		AddString(App()->GetColorTable()->NameAt(i).c_str());
	}
	return 0;
}

void CmColorComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// This is called once! 

	TEXTMETRIC tm;
	CWindowDC dc(m_hWnd);
	HFONT hUIFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	m_font = std::make_unique<Font>(dc, hUIFont); // save for later use

	RECT wndRect;
	GetWindowRect(&wndRect);

	HFONT hOldFont = dc.SelectFont(hUIFont);
	dc.GetTextMetricsW(&tm);
	lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + ITEM_VMARGIN * 2;
	lpMeasureItemStruct->itemWidth = wndRect.right - wndRect.left;
	dc.SelectFont(hOldFont);
}

void CmColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	if (lpdis->itemID != -1)
	{
		Graphics gr(lpdis->hDC);
		RECT rcItem = lpdis->rcItem;
		int rcW = lpdis->rcItem.right - lpdis->rcItem.left;
		int rcH = lpdis->rcItem.bottom - lpdis->rcItem.top;

		Color backColor, textColor;
		backColor.SetFromCOLORREF(GetSysColor((lpdis->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW));
		textColor.SetFromCOLORREF(GetSysColor((lpdis->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

		gr.FillRectangle(&SolidBrush(backColor), rcItem.left, rcItem.top, rcW, rcH);

		gr.FillRectangle(m_colorTable->ObjectAt(lpdis->itemID).get(),
			rcItem.left + ITEM_LEFTMARGIN,
			rcItem.top + ITEM_VMARGIN,
			ITEM_COLORSAMPLE_WIDTH,
			rcH - ITEM_VMARGIN * 2);

		StringFormat fmt;
		fmt.SetLineAlignment(StringAlignmentCenter);
		gr.DrawString((wchar_t*) lpdis->itemData, -1,
			m_font.get(),
			RectF(REAL(rcItem.left + ITEM_LEFTMARGIN + ITEM_COLORSAMPLE_WIDTH + ITEM_RIGHTMARGIN),
				REAL(rcItem.top), REAL(rcW), REAL(rcH)), &fmt, &SolidBrush(textColor));
	}
}
