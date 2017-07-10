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
#pragma once
#include <windows.h>
#include "gdiplus.h"
#include "banner.h"
#include "AppSettings.h"
#include "MainWindow.h"
#include "debug.h"
#include "ShapeTable.h"
#include "EffectTable.h"
#include "ColorSchemeTable.h"
#include <memory>

const float MM_PER_INCH = 25.4f;

class CartelManiaApp
{
public:	
	CartelManiaApp();
	int						Run();
	
	CManiaMainWnd*			GetMainWindow() const { return m_mainWindow.get(); }
	Banner*					GetBanner() { return m_banner.get(); }
	ColorTable*				GetColorTable() { return &m_colorTable; }
	ShapeTable*				GetShapeTable() { return &m_shapeTable; }
	EffectTable*			GetEffectTable() { return &m_effectTable;  }
	const Gdiplus::Brush*	GetBrushFromColorTable(const std::wstring& name){ return m_colorTable.LookupName(name).get(); }
	CPrinter*				GetPrinter()  { return &m_printer; }
	Gdiplus::Size			GetPaperSizeMm() const;
	Gdiplus::Size			GetPrintableAreaMm() const;
	std::wstring			GetPaperName() const;
	int						GetPaperOrientation() const;
	std::wstring			GetDocumentName() const { return m_documentName;  }
	CDevMode*				GetDevMode() { return &m_devMode;  }
	AppSettings*			GetSettings() { return &m_settings;  }
	std::wstring			GetMeasureStringShort() const;
	CDC*					GetPrinterDC() { return &m_printerDC;  }
	static CartelManiaApp*	Instance() { return s_appPtr;  }
	virtual ~CartelManiaApp();

private:
	
	CartelManiaApp(CartelManiaApp&) = delete;
	CartelManiaApp operator= (CartelManiaApp&) = delete;
	DWORD					SetupPrinter();

	static CartelManiaApp*					s_appPtr;
	CPrinter								m_printer;
	CDC										m_printerDC;
	CDevMode								m_devMode;
	ColorTable								m_colorTable;
	ShapeTable								m_shapeTable;
	EffectTable								m_effectTable;
	ColorSchemeTable						m_colorSchemeTable;
	std::unique_ptr<CManiaMainWnd>			m_mainWindow;
	std::unique_ptr<Banner>					m_banner;
	AppSettings								m_settings;
	std::wstring							m_documentName;
};


//
// The global Application Object
//
inline CartelManiaApp* App() { return CartelManiaApp::Instance(); }
