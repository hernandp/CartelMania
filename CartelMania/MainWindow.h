#pragma once

#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include "ColorSelToolWnd.h"
#include "TextEditDlg.h"
#include "resource.h"

class CManiaMainWnd : public CWindowImpl<CManiaMainWnd, CWindow, CFrameWinTraits>, public CDoubleBufferImpl<CManiaMainWnd>
{
	friend class CDoubleBufferImpl<CManiaMainWnd>;

public:
	DECLARE_WND_CLASS_EX(L"CartelMania_Window_Class", CS_HREDRAW | CS_VREDRAW | CS_OWNDC, COLOR_WINDOW);

	BEGIN_MSG_MAP(CManiaMainWnd)
		CHAIN_MSG_MAP(CDoubleBufferImpl)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)

		COMMAND_ID_HANDLER(ID_CMD_EDITTEXT, OnEditText)
		COMMAND_ID_HANDLER(ID_EDIT_SEL1, OnEditSelLine)
		COMMAND_ID_HANDLER(ID_EDIT_SEL2, OnEditSelLine)
		COMMAND_ID_HANDLER(ID_EDIT_SELECTBOTH, OnEditSelLine)

		COMMAND_ID_HANDLER(ID_DEBUG_DRAWVERTICES, OnDebugDrawVertices)
		COMMAND_ID_HANDLER(ID_DEBUG_DISABLEPATHFILL, OnDebugDisablePathFill)
		COMMAND_ID_HANDLER(ID_DEBUG_DISABLEPATHSUBDIVISION, OnDebugDisablePathSubdivision)
		COMMAND_ID_HANDLER(ID_DEBUG_DRAWBOUNDINGRECTS, OnDebugDrawBoundingRects)

		COMMAND_ID_HANDLER(ID_LAYOUT_SINGLELINE, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_LARGEOVERSMALL1, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_LARGEOVERSMALL2, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_LARGEOVERSMALL3, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_MEDIUMOVERMEDIUM, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_SMALLOVERLARGE1, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_SMALLOVERLARGE2, OnSelectLayout)
		COMMAND_ID_HANDLER(ID_LAYOUT_SMALLOVERLARGE3, OnSelectLayout)

		COMMAND_ID_HANDLER(ID_FX_BLOCK, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SOLID, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_TWOOUTLINES, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SHADEDBLOCK, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SHADOWREAR, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SHADOWFORE, OnSelectFx)

		COMMAND_ID_HANDLER(ID_COLOR_OPEN, OnColorOpen)

		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	std::pair<bool, bool> GetLineSelState() const { return m_lineSelState;  }

private:
	void	DoPaint(CDCHandle hDC);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

	LRESULT OnEditText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditSelLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDebugDrawVertices(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDisablePathFill(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDisablePathSubdivision(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDrawBoundingRects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelectLayout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnSelectFx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

	LRESULT OnColorOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

	void UpdateMenu();

	template <class T, typename ...U> void CManiaMainWnd::ApplyFx(U...);

	CImageListManaged		m_imgList;
	CToolBarCtrl			m_toolbar;
	ColorSelectToolWnd		m_colorSelectToolWnd;
	//TextEditDialog			m_textEditDlg;
	std::pair<bool,bool>	m_lineSelState = { true,true };
};
