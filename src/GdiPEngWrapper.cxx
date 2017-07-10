/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
#include "stdafx.h"
#include "GdipEngWrapper.h"
#include "debug.h"

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
		dprintf(L"Shutting down GDI+...");
		Gdiplus::GdiplusShutdown(m_ulGdipToken);
	}
}

