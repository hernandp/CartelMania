#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <vector>
#include "ColorComboBox.h"
#include "resource.h"
#include "TextFx.h"

class ColorSelectToolWnd : public CWindowImpl<ColorSelectToolWnd>
{
public:
	HWND Create(_In_opt_ HWND hWndParent);
	void UpdateUI();

private:
	BEGIN_MSG_MAP_EX(ColorSelectToolWnd)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_MOVE(OnMove)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	int		OnCreate(LPCREATESTRUCT);
	void	OnMove(CPoint pt);
	void	CreateControls();

	std::vector<CStatic>							m_labelCtlList;
	std::vector<std::unique_ptr<CmColorComboBox>>	m_comboCtlList;
};