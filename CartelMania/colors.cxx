#include <windows.h>
#include <gdiplus.h>
#include "colors.h"
using namespace Gdiplus;

//-----------------------------------------------------------------------------
//
// Predefined Colors
// 
//-----------------------------------------------------------------------------

// The following are the good ol' BannerMania ones

const Cartelmania::Color Cartelmania::g_Colors[] = {
	{ L"White",			Cartelmania::FillType::Solid,  HatchStyleTotal,		Gdiplus::Color::White,	Gdiplus::Color::White },
	{ L"50% Gray",		Cartelmania::FillType::Hatch,  HatchStyle50Percent,	Gdiplus::Color::White,	Gdiplus::Color::Black },
	{ L"Violet",        Cartelmania::FillType::Hatch,  HatchStyle50Percent, Gdiplus::Color::Red,	Gdiplus::Color::Blue  },
	{ L"Black",			Cartelmania::FillType::Solid,  HatchStyleTotal,		Gdiplus::Color::Black,	Gdiplus::Color::Black }
};