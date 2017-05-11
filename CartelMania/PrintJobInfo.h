#pragma once
#include <atlapp.h>
#include <atlprint.h>

class CManiaMainWnd;
class CmPrintJobInfo : public CPrintJobInfo
{
public:
	virtual bool PrintPage(UINT nPage, HDC hDC) override;
};