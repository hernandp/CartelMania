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

// The following are the good ol' BannerMania ones


const CmBrush g_bmColors[] {
	{ CmBrush(L"White",    make_unique<SolidBrush>(Color::White)) },
	{ CmBrush(L"12.5% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"25% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},	
	{ CmBrush(L"50% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"75% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"87.5% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Black",    make_unique<SolidBrush>(Color::Black)) },
	{ CmBrush(L"Coarse 6.25% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Coarse 12.5% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Coarse 25% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Coarse 50% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Coarse 75% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Coarse 87.5% Gray", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Verticals", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Horizontals", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Slashes", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Backslashes", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Purple 1", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Purple 2", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Purple 3", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Purple 4", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Purple 5 Pure", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Purple 6", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Violet",   make_unique<HatchBrush>(HatchStyle50Percent, Color::Red  , Color::Blue))},
	{ CmBrush(L"Night Sky",   make_unique<HatchBrush>(HatchStyle50Percent, Color::Red  , Color::Blue))},
	{ CmBrush(L"Blue 1", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue 2", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue 3", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue 4", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue 5 Pure", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue 6", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue 7", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Cobalt", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Serpentine", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Emerald", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 1", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 2", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 3", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 4", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 5 Pure", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 6", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Green 7", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Blue-Green", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Aqua", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Lime", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Yellow 1", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Yellow 2", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Yellow 3", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Yellow 4 Pure", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Yellow 5", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Yellow 6", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Olive", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Ash", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Orange 1", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Orange 2", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Orange 3", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Orange 4", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Orange 5 Pure", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Orange 6", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Tan", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Brown", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 1", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 2", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 3", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 4", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 5 Pure", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 6", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Magenta 7", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Light Red", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Red", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))},
	{ CmBrush(L"Dark Red", make_unique<HatchBrush>(HatchStyle50Percent, Color::White, Color::Black))}
};
