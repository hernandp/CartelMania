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
#include "PrintJobInfo.h"
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
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)

		COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)

		COMMAND_ID_HANDLER(ID_CMD_EDITTEXT, OnEditText)
		COMMAND_ID_HANDLER(ID_EDIT_SEL1, OnEditSelLine)
		COMMAND_ID_HANDLER(ID_EDIT_SEL2, OnEditSelLine)
		COMMAND_ID_HANDLER(ID_EDIT_SELECTBOTH, OnEditSelLine)

		COMMAND_ID_HANDLER(ID_CMD_OPENSHAPETOOL, OnOpenShapeTool)
		COMMAND_ID_HANDLER(ID_CMD_LAYOUTSETUPTOOL, OnLayoutSetupTool)

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
		COMMAND_ID_HANDLER(ID_LAYOUT_SCALETOFIT, OnLayoutScaleToFit)

		COMMAND_ID_HANDLER(ID_FX_BLOCK, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SOLID, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_TWOOUTLINES, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SHADEDBLOCK, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SHADOWREAR, OnSelectFx)
		COMMAND_ID_HANDLER(ID_FX_SHADOWFORE, OnSelectFx)

		COMMAND_ID_HANDLER(ID_COLOR_OPEN, OnColorOpen)

		COMMAND_ID_HANDLER(ID_CMD_PRINT, OnPrint)
		COMMAND_ID_HANDLER(ID_CMD_PRINTPRE, OnPrintPreview)
		COMMAND_ID_HANDLER(ID_CMD_PAGESETUP, OnPageSetup)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	std::pair<bool, bool>	GetLineSelState() const { return m_lineSelState; }

	int		GetClientRect(_Out_ LPRECT lpRect) const;
	void	SetPageAreaDADirty() { m_pageDADirty = true; }

private:
	void	DoPaint(CDCHandle hDC);
	void	DrawSelectionMark(Gdiplus::Graphics & gr, const Gdiplus::RectF & rect);
	void	DoPageSetupDialog();
	void	DrawClientArea(CDCHandle hDC);
	bool	GetPageDisplayAreaRect(RECT*);
	void	CalcPixelsPerMm(float& horizontalRatio, float& verticalRatio);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

	LRESULT OnEditText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditSelLine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnDebugDrawVertices(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDisablePathFill(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDisablePathSubdivision(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDebugDrawBoundingRects(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnSelectLayout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnSelectFx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLayoutScaleToFit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);

	LRESULT OnColorOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnLayoutSetupTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled);
	LRESULT OnOpenShapeTool(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPrintPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPageSetup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	void UpdateMenu();

	template <class T, typename ...U> void CManiaMainWnd::ApplyFx(U...);

	CImageListManaged		m_imgList;
	CToolBarCtrl			m_toolbar;
	ColorSelectToolWnd		m_colorSelectToolWnd;
	TextEditToolWnd			m_textEditToolWnd;
	LayoutSetupToolWnd		m_layoutSetupToolWnd;
	ShapeSelectToolWnd		m_shapeSelectToolWnd;
	CPrinter				m_printer;
	CmPrintJobInfo			m_printJobInfo;
	CStatusBarCtrl			m_statusBar;
	std::pair<bool, bool>	m_lineSelState = { true,false };
	RECT					m_cachedPageDA;
	bool					m_pageDADirty = true;
};
