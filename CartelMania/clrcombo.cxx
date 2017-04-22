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
	
	for (int i = 0; i < m_colorCount; ++i)
	{
		AddString(m_colorList[i].GetName().c_str());
	}

	return 0;
}

const int ITEM_LEFTMARGIN = 4;
const int ITEM_RIGHTMARGIN = 4;
const int ITEM_TOPMARGIN = 4;
const int ITEM_BOTTOMMARGIN = 4;
const int ITEM_COLORSAMPLE_WIDTH = 24;
const int ITEM_COLORSAMPLE_HEIGHT = 20;

LRESULT CmColorComboBox::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	

	lpMeasureItemStruct->itemHeight = 24;
	return DefWindowProc();
}

LRESULT CmColorComboBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	Gdiplus::Font font(lpDrawItemStruct->hDC, hFont);

	if (lpDrawItemStruct->itemAction == ODA_DRAWENTIRE && lpDrawItemStruct->itemID != -1)
	{
		Graphics gr(lpDrawItemStruct->hDC);

		gr.FillRectangle(m_colorList[lpDrawItemStruct->itemID].GetBrush(),
			lpDrawItemStruct->rcItem.left + ITEM_LEFTMARGIN,
			lpDrawItemStruct->rcItem.top + 4,
			22,
			22);

		StringFormat fmt;
		gr.DrawString(m_colorList[lpDrawItemStruct->itemID].GetName().c_str(), -1, 
			&font, PointF(lpDrawItemStruct->rcItem.left + 26, lpDrawItemStruct->rcItem.top + 4), &fmt, &SolidBrush(Color::Black));
	}
	

	return 0L;
}
