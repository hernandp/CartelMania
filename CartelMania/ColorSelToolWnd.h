#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <vector>
#include "ColorComboBox.h"

#include "resource.h"
#include "TextRenderer.h"

#define MSG_WM_UPDATE_ENTRIES	WM_USER+1

class ColorSelectToolWnd : public CWindowImpl<ColorSelectToolWnd>
{
public:
	HWND Create(_In_opt_ HWND hWndParent);

private:
	BEGIN_MSG_MAP_EX(ColorSelectToolWnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(MSG_WM_UPDATE_ENTRIES, OnUpdateEntries)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	
	LRESULT OnUpdateEntries(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		return 0;
	}

	int  OnCreate(LPCREATESTRUCT);
	void CreateControls();

	std::vector<CStatic>							m_label;
	std::vector<std::unique_ptr<CmColorComboBox>>	m_combo;
};