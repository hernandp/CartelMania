/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hern�n Di Pietro

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
