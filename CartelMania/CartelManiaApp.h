#pragma once
#include <windows.h>
#include "gdiplus.h"
#include "banner.h"
#include "AppSettings.h"
#include "MainWindow.h"
#include "debug.h"
#include "ShapeTable.h"
#include <memory>

class CartelManiaApp
{
public:	
	CartelManiaApp();
	int						Run();
	
	CManiaMainWnd*			GetMainWindow() const { return m_mainWindow.get(); }
	Banner*					GetBanner() { return &m_banner; }
	ColorTable*				GetColorTable() { return &m_colorTable; }
	ShapeTable*				GetShapeTable() { return &m_shapeTable; }
	const ShapeFunc&		GetCurrentShapeFunc() { return m_shapeTable.LookupName(m_banner.GetShapeName()); }
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
	static CartelManiaApp*	Instance() { return s_appPtr;  }
	virtual ~CartelManiaApp();

private:
	
	CartelManiaApp(CartelManiaApp&) = delete;
	CartelManiaApp operator= (CartelManiaApp&) = delete;
	DWORD					SetupPrinter();

	static CartelManiaApp*					s_appPtr;
	CPrinter								m_printer;
	CDevMode								m_devMode;
	ColorTable								m_colorTable;
	ShapeTable								m_shapeTable;
	std::unique_ptr<CManiaMainWnd>			m_mainWindow;
	Banner									m_banner;
	AppSettings								m_settings;
	std::wstring							m_documentName;
};


//
// The global Application Object
//
inline CartelManiaApp* App() { return CartelManiaApp::Instance(); }
