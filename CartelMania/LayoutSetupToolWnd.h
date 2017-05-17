#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include "resource.h"

class LayoutSetupToolWnd : public CDialogImpl<LayoutSetupToolWnd>
{
public:
	const int IDD = IDD_LAYOUTSETUPTOOL;

private:

	BEGIN_MSG_MAP(LayoutSetupToolWnd)
		MSG_WM_INITDIALOG(OnInitDialog);
		MSG_WM_MOVE(OnMove);
		MSG_WM_CLOSE(OnClose);
		MSG_WM_HSCROLL(OnHScroll);
	END_MSG_MAP()

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnMove(CPoint pos);
	void OnClose();
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar);

	CEdit			m_bannerWidth;
	CEdit			m_bannerHeight;
	CTrackBarCtrl	m_horzFillTrackbar;
	CTrackBarCtrl	m_vertFillTrackbar;
	bool			m_bInitialized = false;
};