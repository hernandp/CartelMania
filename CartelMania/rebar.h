#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include "atlapp.h"
#include "atlctrls.h"
#include "atldlgs.h"
#include "resource.h"

class CmRebar : public CWindowImpl<CmRebar, CReBarCtrl>
{
	BEGIN_MSG_MAP(CmRebar)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

	CEdit			m_editText1, m_editText2;
};

