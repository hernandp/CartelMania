#include "mainwnd.h"
#include "banner.h"

// ---------------------------------------------------------------------------
// Globals/Externs
// ---------------------------------------------------------------------------
extern Banner g_curBanner;

// ---------------------------------------------------------------------------
// Defined in execmenu.cxx
// ---------------------------------------------------------------------------

void ExecMenu(CWindow*, int);
void UpdateMenu(CWindow*);

// ---------------------------------------------------------------------------

LRESULT CManiaMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	DestroyWindow();
	return 0;
}

// ---------------------------------------------------------------------------

LRESULT CManiaMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	PostQuitMessage(0);
	return 0;
}

// ---------------------------------------------------------------------------

LRESULT CManiaMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	UpdateMenu(this);
	return 1L;
}

// ---------------------------------------------------------------------------

LRESULT CManiaMainWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{	
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);
	RECT rc;
	GetClientRect(&rc);
	g_curBanner.PaintOn(hDC, &rc);
	EndPaint(&ps);
	return 0L;
}

// ---------------------------------------------------------------------------

LRESULT CManiaMainWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (HIWORD(wParam) == 0) //from menu? 
	{
		ExecMenu(this, LOWORD(wParam));
	}
	return 0L;
}
