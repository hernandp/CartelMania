#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlframe.h>
#include <memory>
#include <GdiPlus.h>

class CmBrush;

class CmColorComboBox : public CWindowImpl<CmColorComboBox, CComboBox>, public COwnerDraw<CmColorComboBox>
{
public:
	DECLARE_WND_SUPERCLASS(L"CmColorComboBox_WindowClass", GetWndClassName());

	explicit CmColorComboBox(const CmBrush* colorList, size_t colorCount)
		: CWindowImpl<CmColorComboBox,CComboBox>(),
		m_colorList(colorList), m_colorCount(colorCount)
	{
		ATLASSERT(colorList);
		ATLASSERT(colorCount > 0);
	}

	HWND Create(HWND hWndParent, _U_RECT rect, DWORD dwStyle, UINT id);

private:

	BEGIN_MSG_MAP_EX(CmColorComboBox)
		MSG_WM_CREATE(OnCreate)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CmColorComboBox>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	
	int OnCreate(LPCREATESTRUCT);
	void DrawItem(LPDRAWITEMSTRUCT);
	void MeasureItem(LPMEASUREITEMSTRUCT);

	const CmBrush*					m_colorList;
	std::unique_ptr<Gdiplus::Font>	m_font;
	size_t							m_colorCount;
};
