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
