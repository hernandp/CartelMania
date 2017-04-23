#include "stdafx.h"
#include "rebar.h"

LRESULT CmRebar::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	// Text edit band
	//
	
	ATLASSERT(m_editText1.Create(m_hWnd, RECT{ 0,0,16,50 }, nullptr, WS_CHILD | WS_VISIBLE));
	ATLASSERT(m_editText2.Create(m_hWnd, RECT{ 0,0,16,50 }, nullptr, WS_CHILD | WS_VISIBLE));

	REBARBANDINFO rbbiTextEdit{ 0 };
	rbbiTextEdit.cbSize = sizeof(REBARBANDINFO);
	rbbiTextEdit.fMask = RBBIM_CHILD | RBBIM_TEXT | RBBIM_CHILDSIZE;
	rbbiTextEdit.cyMinChild = 16;
	rbbiTextEdit.cxMinChild = 64;
	rbbiTextEdit.hwndChild = m_editText1.m_hWnd;
	rbbiTextEdit.lpText = L"Line 1";

	InsertBand(0, &rbbiTextEdit);

	return 1L;
}

