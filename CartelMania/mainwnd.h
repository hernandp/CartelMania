#pragma once
#include <atlbase.h>
#include <atlwin.h>

class CManiaMainWnd : public CWindowImpl<CManiaMainWnd, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS_EX(L"CartelMania_Window_Class", CS_HREDRAW | CS_VREDRAW | CS_OWNDC, COLOR_WINDOW);

	BEGIN_MSG_MAP(CManiaMainWnd)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint);
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	END_MSG_MAP()

private:

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
};
