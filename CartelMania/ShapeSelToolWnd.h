#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <vector>
#include "resource.h"

class ShapeSelectToolWnd : public CWindowImpl<ShapeSelectToolWnd>
{
public:
	HWND Create(HWND hWndParent, RECT rcInitial);
	void UpdateUI();

private:
	BEGIN_MSG_MAP_EX(ColorSelectToolWnd)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	const int LISTBOX_MARGIN = 2;

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	int  OnCreate(LPCREATESTRUCT);
	void OnWindowPosChanged(LPWINDOWPOS lpwp);
	void CreateControls();
	void OnClose();

	CListBox lb;
}; 
