/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
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
