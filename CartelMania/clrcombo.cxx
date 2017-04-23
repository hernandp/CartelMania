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

LRESULT CmColorComboBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemAction == ODA_DRAWENTIRE && lpDrawItemStruct->itemID != -1)
	{
		Graphics gr(lpDrawItemStruct->hDC);
		RECT rcItem = lpDrawItemStruct->rcItem;
		int rcW = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
		int rcH = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

		gr.FillRectangle(m_colorList[lpDrawItemStruct->itemID].GetBrush(),
			lpDrawItemStruct->rcItem.left + ITEM_LEFTMARGIN,
			lpDrawItemStruct->rcItem.top + ITEM_VMARGIN,
			ITEM_COLORSAMPLE_WIDTH,
			rcH - ITEM_VMARGIN);

		StringFormat fmt;
		fmt.SetLineAlignment(StringAlignmentCenter);
		gr.DrawString(m_colorList[lpDrawItemStruct->itemID].GetName().c_str(), -1,
			m_font.get(),
			RectF(REAL(lpDrawItemStruct->rcItem.left + ITEM_LEFTMARGIN + ITEM_COLORSAMPLE_WIDTH + ITEM_RIGHTMARGIN),
				REAL(rcItem.top), REAL(rcW), REAL(rcH)), &fmt, &SolidBrush(Color::Black));
	}
	return 0L;
}
