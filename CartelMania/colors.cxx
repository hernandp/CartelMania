#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include "colors.h"

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
const CmBrush g_bmColors[] {
	{ CmBrush(L"White",    make_unique<SolidBrush>(bm_White)) },
	{ CmBrush(L"12.5% Gray", make_unique<HatchBrush>(HatchStyle10Percent, bm_Black, bm_White))},
	{ CmBrush(L"25% Gray", make_unique<HatchBrush>(HatchStyle25Percent, bm_Black, bm_White))},	
	{ CmBrush(L"50% Gray", make_unique<HatchBrush>(HatchStyle50Percent, bm_Black, bm_White))},
	{ CmBrush(L"75% Gray", make_unique<HatchBrush>(HatchStyle75Percent, bm_Black, bm_White))},
	{ CmBrush(L"87.5% Gray", make_unique<HatchBrush>(HatchStyle80Percent, bm_Black, bm_White))},
	{ CmBrush(L"Black",    make_unique<SolidBrush>(bm_Black)) },
	// Coarse-brushes are made-up 
	//
	{ CmBrush(L"Coarse 6.25% Gray", make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray3, bm_White))},
	{ CmBrush(L"Coarse 12.5% Gray", make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_GrayM, bm_White))},
	{ CmBrush(L"Coarse 25% Gray", make_unique<HatchBrush>(HatchStyleSmallCheckerBoard,bm_Gray2, bm_Gray3))},
	{ CmBrush(L"Coarse 50% Gray", make_unique<HatchBrush>(HatchStyleSmallCheckerBoard,bm_White, bm_Black))},
	{ CmBrush(L"Coarse 75% Gray", make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray2, bm_Black))},
	{ CmBrush(L"Coarse 87.5% Gray", make_unique<HatchBrush>(HatchStyleSmallCheckerBoard, bm_Gray, bm_Black))},
	{ CmBrush(L"Verticals", make_unique<HatchBrush>(HatchStyleNarrowVertical, bm_White, bm_Black))},
	{ CmBrush(L"Horizontals", make_unique<HatchBrush>(HatchStyleNarrowHorizontal, bm_White, bm_Black))},
	{ CmBrush(L"Slashes", make_unique<HatchBrush>(HatchStyleDarkDownwardDiagonal, bm_White, bm_Black))},
	{ CmBrush(L"Backslashes", make_unique<HatchBrush>(HatchStyleDarkUpwardDiagonal, bm_White, bm_Black))},
	{ CmBrush(L"Purple 1", make_unique<HatchBrush>(HatchStyle25Percent, bm_Purple, bm_White))},
	{ CmBrush(L"Purple 2", make_unique<HatchBrush>(HatchStyle50Percent, bm_Purple, bm_White))},
	{ CmBrush(L"Purple 3", make_unique<HatchBrush>(HatchStyle75Percent, bm_Purple, bm_White))},
	{ CmBrush(L"Purple 4", make_unique<HatchBrush>(HatchStyle80Percent, bm_Purple, bm_White))},
	{ CmBrush(L"Purple 5 Pure", make_unique<SolidBrush>( bm_Purple))},
	{ CmBrush(L"Purple 6", make_unique<HatchBrush>(HatchStyle50Percent, bm_Purple, bm_Black))},
	{ CmBrush(L"Maroon",   make_unique<HatchBrush>(HatchStyle60Percent, bm_Olive, bm_DarkRed))},
	{ CmBrush(L"Violet",   make_unique<HatchBrush>(HatchStyle50Percent, bm_DarkRed, bm_Blue))},
	{ CmBrush(L"Night Sky",   make_unique<HatchBrush>(HatchStyle50Percent, bm_Purple , bm_DarkGreen))},
	{ CmBrush(L"Blue 1", make_unique<HatchBrush>(HatchStyle25Percent, bm_Blue, bm_White))},
	{ CmBrush(L"Blue 2", make_unique<HatchBrush>(HatchStyle50Percent, bm_Blue, bm_White))},
	{ CmBrush(L"Blue 3", make_unique<HatchBrush>(HatchStyle75Percent, bm_Blue, bm_White))},
	{ CmBrush(L"Blue 4", make_unique<HatchBrush>(HatchStyle80Percent, bm_Blue, bm_White))},
	{ CmBrush(L"Blue 5 Pure", make_unique<SolidBrush>(bm_Blue))},
	{ CmBrush(L"Blue 6", make_unique<SolidBrush>(bm_DarkBlue) )},
	{ CmBrush(L"Blue 7", make_unique<HatchBrush>(HatchStyle25Percent, bm_Blue,bm_Black))},
	{ CmBrush(L"Cobalt", make_unique<HatchBrush>(HatchStyle50Percent, bm_DarkBlue, bm_Purple))},
	{ CmBrush(L"Serpentine", make_unique<HatchBrush>(HatchStyle25Percent, bm_DarkBlue, bm_DarkGreen ))},
	{ CmBrush(L"Emerald", make_unique<HatchBrush>(HatchStyle25Percent, bm_Purple, bm_DarkGreen))},
	{ CmBrush(L"Green 1", make_unique<HatchBrush>(HatchStyle50Percent, bm_White, bm_Green))},
	{ CmBrush(L"Green 2", make_unique<HatchBrush>(HatchStyle25Percent, bm_White, bm_Green))},
	{ CmBrush(L"Green 3", make_unique<HatchBrush>(HatchStyle10Percent, bm_White, bm_Green))},
	{ CmBrush(L"Green 4", make_unique<SolidBrush>(bm_Green))},
	{ CmBrush(L"Green 5 Pure", make_unique<HatchBrush>(HatchStyle25Percent, bm_Green, bm_DarkGreen))},
	{ CmBrush(L"Green 6", make_unique<SolidBrush>(bm_DarkGreen))},
	{ CmBrush(L"Green 7", make_unique<HatchBrush>(HatchStyle25Percent, bm_DarkGreen, bm_Black))},
	{ CmBrush(L"Blue-Green", make_unique<HatchBrush>(HatchStyle25Percent, bm_Aqua, bm_DarkGreen))},
	{ CmBrush(L"Aqua", make_unique<SolidBrush>(bm_Aqua))},
	{ CmBrush(L"Lime", make_unique<HatchBrush>(HatchStyle25Percent, bm_Yellow, bm_Green))},
	{ CmBrush(L"Yellow 1", make_unique<HatchBrush>(HatchStyle25Percent,bm_Yellow, bm_White))},
	{ CmBrush(L"Yellow 2", make_unique<HatchBrush>(HatchStyle50Percent,bm_Yellow, bm_White))},
	{ CmBrush(L"Yellow 3", make_unique<HatchBrush>(HatchStyle75Percent, bm_Yellow, bm_White))},
	{ CmBrush(L"Yellow 4 Pure", make_unique<SolidBrush>(bm_Yellow)) },
	{ CmBrush(L"Yellow 5", make_unique<HatchBrush>(HatchStyle25Percent, bm_Black,bm_Yellow))},
	{ CmBrush(L"Yellow 6", make_unique<HatchBrush>(HatchStyle50Percent, bm_Black,bm_Yellow))},
	{ CmBrush(L"Olive", make_unique<SolidBrush>(bm_Olive))},
	{ CmBrush(L"Ash", make_unique<HatchBrush>(HatchStyle25Percent, bm_Purple, bm_Gray))},
	{ CmBrush(L"Orange 1", make_unique<HatchBrush>(HatchStyle25Percent, bm_Orange, bm_White))},
	{ CmBrush(L"Orange 2", make_unique<HatchBrush>(HatchStyle50Percent, bm_Orange, bm_White))},
	{ CmBrush(L"Orange 3", make_unique<HatchBrush>(HatchStyle25Percent, bm_White, bm_Orange))},
	{ CmBrush(L"Orange 4", make_unique<HatchBrush>(HatchStyle10Percent, bm_White, bm_Orange))},
	{ CmBrush(L"Orange 5 Pure", make_unique<SolidBrush>(bm_Orange)) },
	{ CmBrush(L"Orange 6", make_unique<HatchBrush>(HatchStyle50Percent, bm_Orange, bm_Black))},
	{ CmBrush(L"Tan", make_unique<HatchBrush>(HatchStyle50Percent, bm_Red, bm_Green))},
	{ CmBrush(L"Brown", make_unique<HatchBrush>(HatchStyle50Percent, bm_DarkRed, bm_DarkGreen))},
	{ CmBrush(L"Magenta 1", make_unique<HatchBrush>(HatchStyle25Percent, bm_Magenta, bm_White))},
	{ CmBrush(L"Magenta 2", make_unique<HatchBrush>(HatchStyle50Percent, bm_Magenta, bm_White))},
	{ CmBrush(L"Magenta 3", make_unique<HatchBrush>(HatchStyle75Percent, bm_Magenta, bm_White))},
	{ CmBrush(L"Magenta 4", make_unique<HatchBrush>(HatchStyle80Percent, bm_Magenta, bm_White))},
	{ CmBrush(L"Magenta 5 Pure", make_unique<SolidBrush>(bm_Magenta)) },
	{ CmBrush(L"Magenta 6", make_unique<HatchBrush>(HatchStyle50Percent, bm_Magenta, bm_Black))},
	{ CmBrush(L"Magenta 7", make_unique<HatchBrush>(HatchStyle25Percent, bm_Magenta, bm_Black))},
	{ CmBrush(L"Light Red", make_unique<HatchBrush>(HatchStyle25Percent, bm_White, bm_Red))},
	{ CmBrush(L"Red", make_unique<SolidBrush>(bm_Red))},
	{ CmBrush(L"Dark Red", make_unique<SolidBrush>(bm_DarkRed))}
};

const int g_bmColorsCount = sizeof(g_bmColors) / sizeof(g_bmColors[0]);