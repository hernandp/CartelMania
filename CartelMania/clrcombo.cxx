#include "clrcombo.h"
#include <gdiplus.h>

using namespace Gdiplus;

HWND CmColorComboBox::Create(HWND hWndParent, _U_RECT rect, DWORD dwStyle, UINT id)
{
	return CWindowImpl::Create(hWndParent, rect, nullptr,
		CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | CBS_DROPDOWNLIST | WS_VSCROLL | dwStyle, NULL, id, nullptr);
}

LRESULT CmColorComboBox::OnCreate(LPCREATESTRUCT lps)
{
	LRESULT lr = DefWindowProc();
	
	for (size_t i = 0; i < m_colorCount; ++i)
	{
		AddString(m_colorList[i].GetName().c_str());
	}
	return 0;
}

const int ITEM_LEFTMARGIN = 4;
const int ITEM_RIGHTMARGIN = 4;
const int ITEM_VMARGIN = 4;
const int ITEM_COLORSAMPLE_WIDTH = 24;
const int ITEM_COLORSAMPLE_HEIGHT = 20;

LRESULT CmColorComboBox::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// This is called once! 

	TEXTMETRIC tm;
	CWindowDC dc(m_hWnd);
	HFONT hUIFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	m_font = std::make_unique<Font>(dc, hUIFont); // save for later use

	HFONT hOldFont = dc.SelectFont(hUIFont);
	dc.GetTextMetricsW(&tm);
	lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + ITEM_VMARGIN * 2;
	dc.SelectFont(hOldFont);
	return 0L;
}

LRESULT CmColorComboBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis)
{
	if (lpdis->itemID != -1)
	{
		Graphics gr(lpdis->hDC);
		RECT rcItem = lpdis->rcItem;
		int rcW = lpdis->rcItem.right - lpdis->rcItem.left;
		int rcH = lpdis->rcItem.bottom - lpdis->rcItem.top;

		Color backColor, textColor;
		backColor.SetFromCOLORREF(GetSysColor(lpdis->itemState & ODS_SELECTED ? COLOR_HIGHLIGHT : COLOR_WINDOW));
		textColor.SetFromCOLORREF(GetSysColor(lpdis->itemState & ODS_SELECTED ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

		gr.FillRectangle(&SolidBrush(backColor), rcItem.left, rcItem.top, rcW, rcH);

		gr.FillRectangle(m_colorList[lpdis->itemID].GetBrush(),
			rcItem.left + ITEM_LEFTMARGIN,
			rcItem.top + ITEM_VMARGIN,
			ITEM_COLORSAMPLE_WIDTH,
			rcH - ITEM_VMARGIN * 2);

		StringFormat fmt;
		fmt.SetLineAlignment(StringAlignmentCenter);
		gr.DrawString(m_colorList[lpdis->itemID].GetName().c_str(), -1,
			m_font.get(),
			RectF(REAL(rcItem.left + ITEM_LEFTMARGIN + ITEM_COLORSAMPLE_WIDTH + ITEM_RIGHTMARGIN),
				REAL(rcItem.top), REAL(rcW), REAL(rcH)), &fmt, &SolidBrush(textColor));

		if (lpdis->itemState & ODS_FOCUS)
			DrawFocusRect(lpdis->hDC, &lpdis->rcItem);
	}
	
	return 0L;
}
