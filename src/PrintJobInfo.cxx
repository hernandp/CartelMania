#include "stdafx.h"
#include "PrintJobInfo.h"
#include "MainWindow.h"
#include "CartelManiaApp.h"

bool CmPrintJobInfo::PrintPage(UINT nPage, HDC hDC)
{	
	//
	// Total page count is CxPages * CyPages, so the page to print is:
	//    x = nPage % cxPages-1
	//    y = nPage / cxPages


	XASSERT(m_pageCountX > 0);
	XASSERT(m_pageCountY > 0);

	int pageX = nPage % m_pageCountX;
	int pageY = nPage / m_pageCountX;

	dprintf(L"Printing page %d (px=%d,py=%d)\n", nPage, pageX, pageY);

	// Page width/height in pixels 
	int horzRes = GetDeviceCaps(hDC, HORZRES);
	int vertRes = GetDeviceCaps(hDC, VERTRES);

	RECT rc{ 0, 0, horzRes, vertRes };
	App()->GetBanner()->PaintOn(hDC, &rc, pageX, pageY);
	return true;
}
