#include "stdafx.h"
#include "PrintJobInfo.h"
#include "MainWindow.h"
#include "CartelManiaApp.h"

bool CmPrintJobInfo::PrintPage(UINT nPage, HDC hDC)
{	
	// Horz/Vert pixels per inch for the device
	//
	int ppiX = GetDeviceCaps(hDC, LOGPIXELSX);
	int ppiY = GetDeviceCaps(hDC, LOGPIXELSY);

	// Page width/height in device units
	//
	int physicalW = GetDeviceCaps(hDC, PHYSICALWIDTH);		
	int physicalH = GetDeviceCaps(hDC, PHYSICALHEIGHT);

	// Page width/height in pixels 
	int horzRes = GetDeviceCaps(hDC, HORZRES);
	int vertRes = GetDeviceCaps(hDC, VERTRES);

	int physOffsetX = GetDeviceCaps(hDC, PHYSICALOFFSETX);
	int physOffsetY = GetDeviceCaps(hDC, PHYSICALOFFSETY);

	// Page size in inches
	int inchesWidth = horzRes / ppiX;
	int inchesHeight = vertRes / ppiY;

	Gdiplus::Graphics grDisplay(GetDC(App()->GetMainWindow()->m_hWnd));
	Gdiplus::Graphics grPrinter(hDC);
	float printScaleDpiX = grPrinter.GetDpiX() / grDisplay.GetDpiX();
	float printScaleDpiY = grPrinter.GetDpiY() / grDisplay.GetDpiY();
	
	RECT rcWndClient;
	App()->GetMainWindow()->GetClientRect(&rcWndClient);
	Gdiplus::RectF bannerRect = App()->GetBanner()->GetRect(&rcWndClient);

	float bannerWidthIn = bannerRect.Width / grDisplay.GetDpiX();
	float bannerHeightIn = bannerRect.Height / grDisplay.GetDpiY();

	const float bannerRatio = bannerRect.Width / bannerRect.Height;

	RECT rc{ 0, 0, horzRes, vertRes };
	App()->GetBanner()->PaintOn(hDC, &rc);
	return true;
}
