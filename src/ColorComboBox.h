#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlframe.h>
#include <memory>
#include <GdiPlus.h>
#include <vector>
#include "colorTable.h"

class CmColorComboBox : public CWindowImpl<CmColorComboBox, CComboBox>, public COwnerDraw<CmColorComboBox>
{
public:
	DECLARE_WND_SUPERCLASS(L"CmColorComboBox_WindowClass", GetWndClassName());

	explicit CmColorComboBox(ColorTable* colorTable) : CWindowImpl<CmColorComboBox,CComboBox>(),
		m_colorTable(colorTable)
	{		
		ATLASSERT(colorTable);
		ATLASSERT(colorTable->GetCount() > 0);
	}

	HWND Create(HWND hWndParent, _U_RECT rect, DWORD dwStyle, UINT id = 0UL);
	void SetCurSelByColorName(const std::wstring& name);
	std::wstring GetCurSelColorName() const;

	const CComVariant& GetTag()  const { return m_tag;  }

	template<typename T>
	void SetTag(const T& tag) { m_tag = tag;  }

private:

	BEGIN_MSG_MAP_EX(CmColorComboBox)
		MSG_WM_CREATE(OnCreate)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CmColorComboBox>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	
	int OnCreate(LPCREATESTRUCT);
	void DrawItem(LPDRAWITEMSTRUCT);
	void MeasureItem(LPMEASUREITEMSTRUCT);

	CComVariant							m_tag;
	ColorTable*							m_colorTable;
	std::unique_ptr<Gdiplus::Font>		m_font;
};
