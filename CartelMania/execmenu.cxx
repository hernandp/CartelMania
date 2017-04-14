#include <memory>
#include "cmania.h"
#include "resource.h"
//----------------------------------------------------------------------------

extern Banner g_curBanner;
extern bool g_lineSelState[2];

using namespace std;

//----------------------------------------------------------------------------

void UpdateMenu(HWND hwnd)
{
	HMENU hMenu = GetMenu(hwnd);

	if (hMenu)
	{
		HMENU hEditMenu = GetSubMenu(hMenu, 1);
		if (hEditMenu)
		{
			if (g_lineSelState[0] && g_lineSelState[1])
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_CHECKED);
			}
			else if (g_lineSelState[0])
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_CHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_UNCHECKED);
			}
			else if (g_lineSelState[1])
			{
				CheckMenuItem(hEditMenu, ID_EDIT_SELECTBOTH, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL1, MF_UNCHECKED);
				CheckMenuItem(hEditMenu, ID_EDIT_SEL2, MF_CHECKED);
			}
		}
	}
}

//----------------------------------------------------------------------------

void ExecMenu(HWND hWnd, int id)
{
	static const map<int, BannerLayout> menuToLayoutMap{
		{ ID_LAYOUT_SINGLELINE, BannerLayout::SingleLine },
		{ ID_LAYOUT_LARGEOVERSMALL1, BannerLayout::LargeOverSmall1},
		{ ID_LAYOUT_LARGEOVERSMALL2, BannerLayout::LargeOverSmall2},
		{ ID_LAYOUT_LARGEOVERSMALL3, BannerLayout::LargeOverSmall3},
		{ ID_LAYOUT_MEDIUMOVERMEDIUM, BannerLayout::MediumMedium },
		{ ID_LAYOUT_SMALLOVERLARGE1, BannerLayout::SmallOverLarge1 },
		{ ID_LAYOUT_SMALLOVERLARGE2, BannerLayout::SmallOverLarge2 },
		{ ID_LAYOUT_SMALLOVERLARGE3, BannerLayout::SmallOverLarge3 }
	};

	// Handle layouts menu

	if (menuToLayoutMap.find(id) != menuToLayoutMap.end())
	{
		g_curBanner.SetLayout(menuToLayoutMap.at(id));
		InvalidateRect(hWnd, nullptr, FALSE);
		return;
	}

	switch (id)
	{
		// File 

		case ID_FILE_EXIT:
			if (IDOK == (
				MessageBox(hWnd, L"Do you want to exit CartelMania?", L"Exit", MB_ICONQUESTION | MB_OKCANCEL)))
			{
				CloseWindow(hWnd);
				DestroyWindow(hWnd);
			}

			break;

		// Edit

		case ID_EDIT_SEL1:
			g_lineSelState[0] = true;
			g_lineSelState[1] = false;
			UpdateMenu(hWnd);
			break;

		case ID_EDIT_SEL2:
			g_lineSelState[0] = false;
			g_lineSelState[1] = true;
			UpdateMenu(hWnd);
			break;

		case ID_EDIT_SELECTBOTH:
			g_lineSelState[0] = true;
			g_lineSelState[1] = true;
			UpdateMenu(hWnd);
			break;

		// Layout

		// (see above)

		// Effect

		case ID_CHOOSEFONT:
		{
			CHOOSEFONT cf;
			LOGFONT lf;
			ZeroMemory(&cf, sizeof(CHOOSEFONT));
			ZeroMemory(&lf, sizeof(LOGFONT));

			cf.lStructSize = sizeof(CHOOSEFONT);
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &lf;
			cf.Flags = CF_TTONLY | CF_APPLY | CF_FORCEFONTEXIST | CF_NOSCRIPTSEL | CF_NOSIZESEL;
			if (ChooseFont(&cf))
			{
				//g_curBanner.SetFont(cf.lpLogFont->lfFaceName);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		}

		case ID_FX_SOLID:
			g_curBanner.GetLine(0).SetTextFx(make_unique<TextFxSolid>());
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case ID_FX_THICK:
		{
			auto textR = make_unique<TextFxSolid>();
			textR->SetOutlineWidth(6.0f);
			g_curBanner.GetLine(0).SetTextFx(move(textR));
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case ID_FX_TWOOUTLINES:
		{
			auto textR = make_unique<TextFxTwoOutlines>();
			textR->SetOutlineWidth(6.0f);
			textR->SetOutline2Width(6.0f);
			g_curBanner.GetLine(0).SetTextFx(move(textR));
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

		case ID_FX_VERTICAL:
			break;

		case ID_FX_BLOCK:
			g_curBanner.GetLine(0).SetTextFx(make_unique<TextFxBlock>());
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case ID_FX_SHADOWREAR:
			g_curBanner.GetLine(0).SetTextFx(make_unique<TextFxShadowRear>());
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		// Shape

			
	}
}
