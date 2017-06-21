#pragma once
#include <windows.h>
#include "gdiplus.h"
#include "banner.h"
#include "AppSettings.h"
#include "MainWindow.h"
#include "debug.h"
#include "ShapeTable.h"
#include "EffectTable.h"
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
	const ShapeFunc&		GetCurrentShapeFunc() { return m_shapeTable.LookupName(m_banner->GetShapeName()); }
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
	CDC										m_printerDC;
	CDevMode								m_devMode;
	ColorTable								m_colorTable;
	ShapeTable								m_shapeTable;
	EffectTable								m_effectTable;
	std::unique_ptr<CManiaMainWnd>			m_mainWindow;
	std::unique_ptr<Banner>					m_banner;
	AppSettings								m_settings;
	std::wstring							m_documentName;
};


//
// The global Application Object
//
inline CartelManiaApp* App() { return CartelManiaApp::Instance(); }
