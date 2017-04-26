#include "stdafx.h"
#include "GdipEngWrapper.h"

GdiPlusEngine::GdiPlusEngine() : m_initOk(false), m_gdipStartupInput(0), m_ulGdipToken(0)
{
	if (Gdiplus::GdiplusStartup(&m_ulGdipToken, &m_gdipStartupInput, NULL) ==
		Gdiplus::Status::Ok)
	{
		m_initOk = true;
	}
}

GdiPlusEngine::~GdiPlusEngine()
{
	if (m_initOk && m_ulGdipToken)
	{
		Gdiplus::GdiplusShutdown(m_ulGdipToken);
	}
}

