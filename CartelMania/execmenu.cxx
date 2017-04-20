#include <atlbase.h>
#include <atlwin.h>
#include <memory>
#include "cmania.h"
#include "resource.h"


//----------------------------------------------------------------------------

extern Banner g_curBanner;
extern GlobalSettings g_globalSettings;
extern bool g_lineSelState[2];

using namespace std;

//----------------------------------------------------------------------------

void UpdateMenu(CWindow* pWnd)
{
	HMENU hMenu = pWnd->GetMenu();

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

		HMENU hDebugMenu = GetSubMenu(hMenu, 6);
		if (hDebugMenu)
		{
			CheckMenuItem(hDebugMenu, ID_DEBUG_DRAWVERTICES, g_globalSettings.m_fDebugDrawVertices ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHFILL, g_globalSettings.m_fDebugDisableFillPath ? MF_CHECKED : MF_UNCHECKED);
			CheckMenuItem(hDebugMenu, ID_DEBUG_DISABLEPATHSUBDIVISION, g_globalSettings.m_fDisableSubdiv ? MF_CHECKED : MF_UNCHECKED);
		}
	}
}

//----------------------------------------------------------------------------

void ExecMenu(CWindow* pWnd, int id)
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
		pWnd->InvalidateRect(nullptr, FALSE);
		return;
	}

	switch (id)
	{
		// File 

		case ID_FILE_EXIT:
			if (IDOK == (
				pWnd->MessageBoxW(L"Do you want to exit CartelMania?", L"Exit", MB_ICONQUESTION | MB_OKCANCEL)))
			{
				CloseWindow(pWnd->m_hWnd);
				DestroyWindow(pWnd->m_hWnd);
			}

			break;

			// Edit

		case ID_EDIT_SEL1:
			g_lineSelState[0] = true;
			g_lineSelState[1] = false;
			UpdateMenu(pWnd);
			break;

		case ID_EDIT_SEL2:
			g_lineSelState[0] = false;
			g_lineSelState[1] = true;
			UpdateMenu(pWnd);
			break;

		case ID_EDIT_SELECTBOTH:
			g_lineSelState[0] = true;
			g_lineSelState[1] = true;
			UpdateMenu(pWnd);
			break;

			// Layout

			// (see above)

			// Effect

		case ID_CHOOSEFONT:
		{
			//CFontDialog cf;
			//cf.RunModalLoop();
			//CHOOSEFONT cf;
			//LOGFONT lf;
			//ZeroMemory(&cf, sizeof(CHOOSEFONT));
			//ZeroMemory(&lf, sizeof(LOGFONT));

			//cf.lStructSize = sizeof(CHOOSEFONT);
			//cf.hwndOwner = hWnd;
			//cf.lpLogFont = &lf;
			//cf.Flags = CF_TTONLY | CF_APPLY | CF_FORCEFONTEXIST | CF_NOSCRIPTSEL | CF_NOSIZESEL;
			//if (ChooseFont(&cf))
			//{
			//	//g_curBanner.SetFont(cf.lpLogFont->lfFaceName);
			//	InvalidateRect(hWnd, NULL, FALSE);
			//}
			break;
		}

		case ID_FX_SOLID:
			g_curBanner.GetTopLine()->SetTextFx(make_unique<TextFxSolid>());
			pWnd->InvalidateRect(nullptr, FALSE);
			break;

		case ID_FX_THICK:
		{
			auto textR = make_unique<TextFxSolid>();
			textR->SetOutlineWidth(6.0f);
			g_curBanner.GetTopLine()->SetTextFx(move(textR));
			pWnd->InvalidateRect(nullptr, FALSE);
			break;
		}

		case ID_FX_TWOOUTLINES:
		{
			auto textR = make_unique<TextFxTwoOutlines>();
			textR->SetOutlineWidth(6.0f);
			textR->SetOutline2Width(6.0f);
			g_curBanner.GetTopLine()->SetTextFx(move(textR));
			pWnd->InvalidateRect(nullptr, FALSE);
			break;
		}

		case ID_FX_VERTICAL:
			break;

		case ID_FX_BLOCK:
			g_curBanner.GetTopLine()->SetTextFx(make_unique<TextFxBlock>());
			pWnd->InvalidateRect(nullptr, FALSE);
			break;

		case ID_FX_SHADOWREAR:
			g_curBanner.GetTopLine()->SetTextFx(make_unique<TextFxShadowRear>());
			pWnd->InvalidateRect(nullptr, FALSE);
			break;

			// Shape

			// Debug

		case ID_DEBUG_DRAWVERTICES:
			g_globalSettings.m_fDebugDrawVertices = !g_globalSettings.m_fDebugDrawVertices;
			pWnd->InvalidateRect(nullptr, FALSE);
			UpdateMenu(pWnd);
			break;

		case ID_DEBUG_DISABLEPATHFILL:
			g_globalSettings.m_fDebugDisableFillPath = !g_globalSettings.m_fDebugDisableFillPath;
			pWnd->InvalidateRect(nullptr, FALSE);
			UpdateMenu(pWnd);
			break;

		case ID_DEBUG_DISABLEPATHSUBDIVISION:
			g_globalSettings.m_fDisableSubdiv = !g_globalSettings.m_fDisableSubdiv;
			//g_curBanner.Invalidate();
			pWnd->InvalidateRect(nullptr, FALSE);
			UpdateMenu(pWnd);
			break;
	}
}
