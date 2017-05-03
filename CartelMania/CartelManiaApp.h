#pragma once

#include "GdiPEngWrapper.h"
#include "banner.h"
#include "AppSettings.h"
#include "MainWindow.h"
#include <memory>

class CartelManiaApp
{
public:	
	static CartelManiaApp* GetInstance()
	{
		return (m_inst == nullptr) ? (m_inst = new CartelManiaApp) : m_inst;
	}

	int						Run();
	CManiaMainWnd*			GetMainWindow() const { return m_mainWindow.get(); }
	Banner*					GetBanner() { return &m_banner; }
	AppSettings*			GetSettings() { return &m_settings;  }
	
private:
	CartelManiaApp();
	CartelManiaApp(CartelManiaApp&) = delete;
	CartelManiaApp operator= (CartelManiaApp&) = delete;

	static CartelManiaApp*					m_inst;
	std::unique_ptr<CManiaMainWnd>			m_mainWindow;
	GdiPlusEngine							m_gdipEng;
	Banner									m_banner;
	AppSettings								m_settings;
};

inline CartelManiaApp* CmApp()
{
	return CartelManiaApp::GetInstance();
}
