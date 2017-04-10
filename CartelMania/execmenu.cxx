#include <memory>
#include "cmania.h"
#include "resource.h"

extern Banner g_curBanner;
extern bool g_lineSelState[2];

using namespace std;

void ExecMenu(HWND hWnd, int id)
{
	switch (id)
	{
		case ID_FILE_EXIT:
			if (IDOK == (
				MessageBox(hWnd, L"Do you want to exit CartelMania?", L"Exit", MB_ICONQUESTION | MB_OKCANCEL)))
			{
				CloseWindow(hWnd);
				DestroyWindow(hWnd);
			}

			break;

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
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}

		case ID_FX_SOLID:
			g_curBanner.GetLine(0).SetTextFx(make_unique<TextFxSolid>());
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_FX_THICK:
		{
			auto textR = make_unique<TextFxSolid>();
			textR->SetOutlineWidth(6.0f);
			g_curBanner.GetLine(0).SetTextFx(move(textR));
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		case ID_FX_TWOOUTLINES:
		{
			auto textR = make_unique<TextFxTwoOutlines>();
			textR->SetOutlineWidth(6.0f);
			textR->SetOutline2Width(6.0f);
			g_curBanner.GetLine(0).SetTextFx(move(textR));
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		case ID_FX_VERTICAL:
			break;



		case ID_FX_BLOCK:
			g_curBanner.GetLine(0).SetTextFx(make_unique<TextFxBlock>());
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_FX_SHADOWREAR:
			g_curBanner.GetLine(0).SetTextFx(make_unique<TextFxShadowRear>());
			InvalidateRect(hWnd, NULL, TRUE);
			break;
	}
}