#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <vector>
#include "ColorComboBox.h"

#include "resource.h"

class ColorSelectToolWnd : public CWindowImpl<ColorSelectToolWnd>
{
public:
	HWND Create(_In_opt_ HWND hWndParent);

private:
	BEGIN_MSG_MAP_EX(ColorSelectToolWnd)
		MSG_WM_CREATE(OnCreate)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	int OnCreate(LPCREATESTRUCT);

	std::vector<CStatic>							m_label;
	std::vector<std::unique_ptr<CmColorComboBox>>	m_combo;
};