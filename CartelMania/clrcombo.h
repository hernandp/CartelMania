#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atluser.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include "colors.h"

class CmColorComboBox : public CWindowImpl<CmColorComboBox, CComboBox>
{
public:
	explicit CmColorComboBox(const CmBrush* colorList, size_t colorCount)
		: m_colorList(colorList), m_colorCount(colorCount)
	{
		ATLASSERT(colorList);
		ATLASSERT(colorCount > 0);
	}

	HWND Create(HWND hWndParent, _U_RECT rect, DWORD dwStyle, UINT id);

private:

	BEGIN_MSG_MAP_EX(CmColorComboBox)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DRAWITEM(OnDrawItem)
		MSG_WM_MEASUREITEM(OnMeasureItem)
	END_MSG_MAP()

	LRESULT OnCreate(LPCREATESTRUCT);
	LRESULT OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	LRESULT OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	const CmBrush* m_colorList;
	size_t	 m_colorCount;
};
