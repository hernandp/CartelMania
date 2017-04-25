#include "stdafx.h"
#include "colors.h"
#include "debug.h"

using namespace Gdiplus;
using namespace std;

//-----------------------------------------------------------------------------
//
// Predefined Colors
// 
//-----------------------------------------------------------------------------

//
// BannerMania-style Palette
//
const Color bm_White{ 255,255,255 };
const Color bm_Purple{ 80,0,80 };
const Color bm_Red{ 248,0,80 };
const Color bm_Blue{ 0,0,248 };
const Color bm_DarkBlue{ 0, 0, 168 };
const Color bm_DarkRed{ 168,0,0 };
const Color bm_Magenta{ 248,0,168 };
const Color bm_Orange{ 248,84,0 };
const Color bm_Green{ 0,168,0 };
const Color bm_DarkGreen{ 0,84,0 };
const Color bm_Aqua{ 0,168,168 };
const Color bm_Yellow{ 248,252,0 };
const Color bm_Olive{ 80,84,0 };
const Color bm_Gray2{ 168,168,168 };
const Color bm_GrayM{ 200,200,200 };
const Color bm_Gray3{ 248,248,248 };
const Color bm_Gray{ 80,84,80 };
const Color bm_Black{ 0,0,0 };

//
// Color table
//

ColorTable_T g_colorTable;

void InitColorTable()
{
	g_colorTable.push_back(make_pair(L"White", make_unique<SolidBrush>(bm_White)));
	g_colorTable.push_back(make_pair(L"12.5% Gray",  make_unique<HatchBrush>(HatchStyle10Percent, bm_Black, bm_White)));
	g_colorTable.push_back(make_pair(L"25% Gray",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Black, bm_White)));
	g_colorTable.push_back(make_pair(L"50% Gray",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Black, bm_White)));
	g_colorTable.push_back(make_pair(L"75% Gray",  make_unique<HatchBrush>(HatchStyle75Percent, bm_Black, bm_White)));
	g_colorTable.push_back(make_pair(L"87.5% Gray",  make_unique<HatchBrush>(HatchStyle80Percent, bm_Black, bm_White)));
	g_colorTable.push_back(make_pair(L"Black",  make_unique<SolidBrush>(bm_Black)));
	// Coarse-brushes are made-up 
	//
	g_colorTable.push_back(make_pair(L"Coarse 6.25% Gray",  make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray3, bm_White)));
	g_colorTable.push_back(make_pair(L"Coarse 12.5% Gray",  make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_GrayM, bm_White)));
	g_colorTable.push_back(make_pair(L"Coarse 25% Gray",  make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray2, bm_Gray3)));
	g_colorTable.push_back(make_pair(L"Coarse 50% Gray",  make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_White, bm_Black)));
	g_colorTable.push_back(make_pair(L"Coarse 75% Gray",  make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray2, bm_Black)));
	g_colorTable.push_back(make_pair(L"Coarse 87.5% Gray",  make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray, bm_Black)));
	g_colorTable.push_back(make_pair(L"Verticals",  make_unique<HatchBrush>(HatchStyleNarrowVertical, bm_White, bm_Black)));
	g_colorTable.push_back(make_pair(L"Horizontals",  make_unique<HatchBrush>(HatchStyleNarrowHorizontal, bm_White, bm_Black)));
	g_colorTable.push_back(make_pair(L"Slashes",  make_unique<HatchBrush>(HatchStyleDarkDownwardDiagonal, bm_White, bm_Black)));
	g_colorTable.push_back(make_pair(L"Backslashes",  make_unique<HatchBrush>(HatchStyleDarkUpwardDiagonal, bm_White, bm_Black)));
	g_colorTable.push_back(make_pair(L"Purple 1",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Purple, bm_White)));
	g_colorTable.push_back(make_pair(L"Purple 2",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Purple, bm_White)));
	g_colorTable.push_back(make_pair(L"Purple 3",  make_unique<HatchBrush>(HatchStyle75Percent, bm_Purple, bm_White)));
	g_colorTable.push_back(make_pair(L"Purple 4",  make_unique<HatchBrush>(HatchStyle80Percent, bm_Purple, bm_White)));
	g_colorTable.push_back(make_pair(L"Purple 5 Pure",  make_unique<SolidBrush>(bm_Purple)));
	g_colorTable.push_back(make_pair(L"Purple 6",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Purple, bm_Black)));
	g_colorTable.push_back(make_pair(L"Maroon",  make_unique<HatchBrush>(HatchStyle60Percent, bm_Olive, bm_DarkRed)));
	g_colorTable.push_back(make_pair(L"Violet",  make_unique<HatchBrush>(HatchStyle50Percent, bm_DarkRed, bm_Blue)));
	g_colorTable.push_back(make_pair(L"Night Sky",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Purple, bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Blue 1",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Blue, bm_White)));
	g_colorTable.push_back(make_pair(L"Blue 2",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Blue, bm_White)));
	g_colorTable.push_back(make_pair(L"Blue 3",  make_unique<HatchBrush>(HatchStyle75Percent, bm_Blue, bm_White)));
	g_colorTable.push_back(make_pair(L"Blue 4",  make_unique<HatchBrush>(HatchStyle80Percent, bm_Blue, bm_White)));
	g_colorTable.push_back(make_pair(L"Blue 5 Pure",  make_unique<SolidBrush>(bm_Blue)));
	g_colorTable.push_back(make_pair(L"Blue 6",  make_unique<SolidBrush>(bm_DarkBlue)));
	g_colorTable.push_back(make_pair(L"Blue 7",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Blue, bm_Black)));
	g_colorTable.push_back(make_pair(L"Cobalt",  make_unique<HatchBrush>(HatchStyle50Percent, bm_DarkBlue, bm_Purple)));
	g_colorTable.push_back(make_pair(L"Serpentine",  make_unique<HatchBrush>(HatchStyle25Percent, bm_DarkBlue, bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Emerald",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Purple, bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Green 1",  make_unique<HatchBrush>(HatchStyle50Percent, bm_White, bm_Green)));
	g_colorTable.push_back(make_pair(L"Green 2",  make_unique<HatchBrush>(HatchStyle25Percent, bm_White, bm_Green)));
	g_colorTable.push_back(make_pair(L"Green 3",  make_unique<HatchBrush>(HatchStyle10Percent, bm_White, bm_Green)));
	g_colorTable.push_back(make_pair(L"Green 4",  make_unique<SolidBrush>(bm_Green)));
	g_colorTable.push_back(make_pair(L"Green 5 Pure",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Green, bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Green 6",  make_unique<SolidBrush>(bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Green 7",  make_unique<HatchBrush>(HatchStyle25Percent, bm_DarkGreen, bm_Black)));
	g_colorTable.push_back(make_pair(L"Blue-Green",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Aqua, bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Aqua",  make_unique<SolidBrush>(bm_Aqua)));
	g_colorTable.push_back(make_pair(L"Lime",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Yellow, bm_Green)));
	g_colorTable.push_back(make_pair(L"Yellow 1",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Yellow, bm_White)));
	g_colorTable.push_back(make_pair(L"Yellow 2",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Yellow, bm_White)));
	g_colorTable.push_back(make_pair(L"Yellow 3",  make_unique<HatchBrush>(HatchStyle75Percent, bm_Yellow, bm_White)));
	g_colorTable.push_back(make_pair(L"Yellow 4 Pure",  make_unique<SolidBrush>(bm_Yellow)));
	g_colorTable.push_back(make_pair(L"Yellow 5",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Black, bm_Yellow)));
	g_colorTable.push_back(make_pair(L"Yellow 6",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Black, bm_Yellow)));
	g_colorTable.push_back(make_pair(L"Olive",  make_unique<SolidBrush>(bm_Olive)));
	g_colorTable.push_back(make_pair(L"Ash",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Purple, bm_Gray)));
	g_colorTable.push_back(make_pair(L"Orange 1",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Orange, bm_White)));
	g_colorTable.push_back(make_pair(L"Orange 2",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Orange, bm_White)));
	g_colorTable.push_back(make_pair(L"Orange 3",  make_unique<HatchBrush>(HatchStyle25Percent, bm_White, bm_Orange)));
	g_colorTable.push_back(make_pair(L"Orange 4",  make_unique<HatchBrush>(HatchStyle10Percent, bm_White, bm_Orange)));
	g_colorTable.push_back(make_pair(L"Orange 5 Pure",  make_unique<SolidBrush>(bm_Orange)));
	g_colorTable.push_back(make_pair(L"Orange 6",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Orange, bm_Black)));
	g_colorTable.push_back(make_pair(L"Tan",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Red, bm_Green)));
	g_colorTable.push_back(make_pair(L"Brown",  make_unique<HatchBrush>(HatchStyle50Percent, bm_DarkRed, bm_DarkGreen)));
	g_colorTable.push_back(make_pair(L"Magenta 1",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Magenta, bm_White)));
	g_colorTable.push_back(make_pair(L"Magenta 2",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Magenta, bm_White)));
	g_colorTable.push_back(make_pair(L"Magenta 3",  make_unique<HatchBrush>(HatchStyle75Percent, bm_Magenta, bm_White)));
	g_colorTable.push_back(make_pair(L"Magenta 4",  make_unique<HatchBrush>(HatchStyle80Percent, bm_Magenta, bm_White)));
	g_colorTable.push_back(make_pair(L"Magenta 5 Pure",  make_unique<SolidBrush>(bm_Magenta)));
	g_colorTable.push_back(make_pair(L"Magenta 6",  make_unique<HatchBrush>(HatchStyle50Percent, bm_Magenta, bm_Black)));
	g_colorTable.push_back(make_pair(L"Magenta 7",  make_unique<HatchBrush>(HatchStyle25Percent, bm_Magenta, bm_Black)));
	g_colorTable.push_back(make_pair(L"Light Red",  make_unique<HatchBrush>(HatchStyle25Percent, bm_White, bm_Red)));
	g_colorTable.push_back(make_pair(L"Red",  make_unique<SolidBrush>(bm_Red)));
	g_colorTable.push_back(make_pair(L"Dark Red",  make_unique<SolidBrush>(bm_DarkRed)));
}

const Gdiplus::Brush* GetBrushFromColorTable(const std::wstring & name)
{
	auto ret = std::find_if ( g_colorTable.cbegin(), g_colorTable.cend(), 
		[&name](const std::pair<std::wstring, std::unique_ptr<Gdiplus::Brush>>& entry)
	{
		return entry.first == name;
	}	
	);

	if (ret == g_colorTable.end())
	{
		XASSERT(FALSE);
		return nullptr;
	}

	return ret->second.get();
}

//-----------------------------------------------------------------------------

Gdiplus::Color MakeRandColor()
{
	return Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
}

//-----------------------------------------------------------------------------