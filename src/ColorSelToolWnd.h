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
#include <vector>
#include "ColorComboBox.h"
#include "resource.h"
#include "TextFx.h"

class ColorSelectToolWnd : public CWindowImpl<ColorSelectToolWnd>
{
public:
	HWND Create(_In_ HWND hWndParent);
	void UpdateUI();

private:
	BEGIN_MSG_MAP_EX(ColorSelectToolWnd)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_CLOSE(OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	int		OnCreate(LPCREATESTRUCT);
	void	OnMove(CPoint pt);
	void	CreateControls();
	void	OnClose();
	void	OnWindowPosChanged(LPWINDOWPOS lpwPos);

	std::vector<CStatic>							m_labelCtlList;
	std::vector<std::unique_ptr<CmColorComboBox>>	m_comboCtlList;
};