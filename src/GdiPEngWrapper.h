#pragma once
#include <gdiplus.h>

//----------------------------------------------------------------------------
// 
// Encapsulates GDI+ engine for auto-destruction at exit
//
//----------------------------------------------------------------------------
class GdiPlusEngine
{
public:
	GdiPlusEngine();
	~GdiPlusEngine();
	bool IsInitOk() const { return m_initOk; };

private:
	bool							m_initOk;
	Gdiplus::GdiplusStartupInput	m_gdipStartupInput;
	ULONG_PTR						m_ulGdipToken;
};
