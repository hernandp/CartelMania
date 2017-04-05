#ifndef _FONT_H_
#define _FONT_H_
#include <windows.h>

// ---------------------------------------------------------------------------
//
// Creates a font with an appropiate size for a text to fill a rectangle
//
// ---------------------------------------------------------------------------
HFONT CreateFont2FillTextRect(HDC hDC, const RECT& rc, LPCWSTR lpString)
{
	LOGFONT lf;
	LONG rcWidth = rc.right - rc.left;
	LONG rcHeight = rc.bottom - rc.top;
	int  len = lstrlen(lpString);
	int cxLogPixels = GetDeviceCaps(hDC, LOGPIXELSX);
	int cyLogPixels = GetDeviceCaps(hDC, LOGPIXELSY);

	lf.lfWidth = -MulDiv(rcWidth / len, cxLogPixels, 72);
	lf.lfHeight = -MulDiv(rcHeight / len, cyLogPixels, 72);

	return NULL;
}

#endif //FONT_H_
