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
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include "ColorSelToolWnd.h"
#include "TextEditToolWnd.h"
#include "ShapeSelToolWnd.h"
#include "LayoutSetupToolWnd.h"
#include "LineLayoutToolWnd.h"
#include "PrintJobInfo.h"
#include "EffectToolWnd.h"
#include "resource.h"

class CManiaMainWnd : public CWindowImpl<CManiaMainWnd, CWindow, CFrameWinTraits>, public CDoubleBufferImpl<CManiaMainWnd>
{
	friend class CDoubleBufferImpl<CManiaMainWnd>;

public:
	DECLARE_WND_CLASS_EX(L"CartelMania_Window_Class", CS_HREDRAW | CS_VREDRAW | CS_OWNDC, COLOR_MENUHILIGHT);

	BEGIN_MSG_MAP(CManiaMainWnd)
		CHAIN_MSG_MAP(CDoubleBufferImpl)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)

		COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)

		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)

		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)

		COMMAND_ID_HANDLER(ID_CMD_EDITTEXT, OnEditText)

		COMMAND_ID_HANDLER(ID_CMD_OPENSHAPETOOL, OnOpenShapeTool)
		COMMAND_ID_HANDLER(ID_CMD_LAYOUTSETUPTOOL, OnLayoutSetupTool)
		COMMAND_ID_HANDLER(ID_CMD_LINELAYOUTTOOL, OnLineLayoutTool)
		COMMAND_ID_HANDLER(ID_CMD_COLORTOOL, OnOpenColorTool)
		COMMAND_ID_HANDLER(ID_CMD_EFFECTTOOL, OnOpenEffectTool)

		COMMAND_ID_HANDLER(ID_DEBUG_DRAWVERTICES, OnDebugDrawVertices)
		COMMAND_ID_HANDLER(ID_DEBUG_DISABLEPATHFILL, OnDebugDisablePathFill)
		COMMAND_ID_HANDLER(ID_DEBUG_DISABLEPATHSUBDIVISION, OnDebugDisablePathSubdivision)
		COMMAND_ID_HANDLER(ID_DEBUG_DRAWBOUNDINGRECTS, OnDebugDrawBoundingRects)

		COMMAND_ID_HANDLER(ID_CMD_PRINT, OnPrint)
		COMMAND_ID_HANDLER(ID_CMD_PRINTPRE, OnPrintPreview)
		COMMAND_ID_HANDLER(ID_CMD_PAGESETUP, OnPageSetup)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	std::pair<bool, bool>	GetLineSelState() const { return m_lineSelState; }
	int						GetClientRect(_Out_ LPRECT lpRect) const;
	void					SetPageAreaDADirty() { m_pageDADirty = true; }
	BannerLine*				GetBannerLineFromSelState();
	void					NotifyToolboxClose(HWND hWnd);

private:
	void	DoPaint(CDCHandle hDC);
	void	DrawSelectionMark(Gdiplus::Graphics & gr, const Gdiplus::RectF & rect);
	void	DoPageSetupDialog();
	void	DrawClientArea(CDCHandle hDC);
	bool	GetPageDisplayAreaRect(RECT*);
	void	CalcPixelsPerMm(float& horizontalRatio, float& verticalRatio);
	void	NotifyActiveToolboxes();
	void	InvalidatePageDA();

	LRESULT OnFileNew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	
	LRESULT OnEditText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDebugDrawVertices(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDisablePathFill(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDisablePathSubdivision(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDrawBoundingRects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnOpenEffectTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnOpenColorTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLayoutSetupTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnOpenShapeTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnLineLayoutTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPrintPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPageSetup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	void UpdateMenu();
	
	CImageListManaged		m_imgList;
	CToolBarCtrl			m_toolbar;
	ColorSelectToolWnd		m_colorSelectToolWnd;
	TextEditToolWnd			m_textEditToolWnd;
	LayoutSetupToolWnd		m_layoutSetupToolWnd;
	LineLayoutToolWnd		m_lineLayoutToolWnd;
	ShapeSelectToolWnd		m_shapeSelectToolWnd;
	EffectToolWnd			m_effectToolWnd;
	CPrinter				m_printer;
	CmPrintJobInfo			m_printJobInfo;
	CStatusBarCtrl			m_statusBar;
	std::pair<bool, bool>	m_lineSelState = { true,false };
	RECT					m_cachedPageDA;
	bool					m_pageDADirty = true;
};
