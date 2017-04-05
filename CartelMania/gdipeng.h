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
	GdiPlusEngine() : m_initOk(false), m_gdipStartupInput(0), m_ulGdipToken(0)
	{
		if (Gdiplus::GdiplusStartup(&m_ulGdipToken, &m_gdipStartupInput, NULL) ==
			Gdiplus::Status::Ok)
		{
			m_initOk = true;
		}
	}

	~GdiPlusEngine()
	{
		if (m_initOk && m_ulGdipToken)
		{
			Gdiplus::GdiplusShutdown(m_ulGdipToken);
		}
	}

	bool IsInitOk() const { return m_initOk; };

private:
	bool							m_initOk;
	Gdiplus::GdiplusStartupInput	m_gdipStartupInput;
	ULONG_PTR						m_ulGdipToken;
};
