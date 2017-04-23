#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include "resource.h"

class CTextEditDialog : public CDialogImpl<CTextEditDialog>
{
	BEGIN_MSG_MAP(CTextEditDialog)

	END_MSG_MAP()
public:
	const int IDD = IDD_TEXTEDIT;
};