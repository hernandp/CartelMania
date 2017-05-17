#include "stdafx.h"
#include "CartelManiaApp.h"
#include "ShapeSelToolWnd.h"

HWND ShapeSelectToolWnd::Create(HWND hWndParent, RECT rcInitial)
{
	return CWindowImpl::Create(hWndParent, rcInitial, L"Shape", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, WS_EX_PALETTEWINDOW);
}

void ShapeSelectToolWnd::CreateControls()
{
	const auto shapeTable = App()->GetShapeTable();
	lb.Create(m_hWnd, rcDefault, 0, WS_CHILD | LBS_NOTIFY | WS_BORDER | WS_VSCROLL | WS_VISIBLE);

	for (int i = 0; i < shapeTable->GetCount(); ++i)
	{
		lb.AddString(shapeTable->NameAt(i).c_str());
	}
	
	lb.SetFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	lb.SetCurSel(0);
}

void ShapeSelectToolWnd::UpdateUI()
{
}

LRESULT ShapeSelectToolWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{	
	if (HIWORD(wParam) == LBN_SELCHANGE)
	{		
		int iSel = lb.GetCurSel();
		if (iSel != LB_ERR)
		{
			auto textBuffer = std::make_unique<wchar_t[]>(lb.GetTextLen(iSel) + 1);
			lb.GetText(iSel, textBuffer.get());
			App()->GetBanner()->SetShapeName(textBuffer.get());
			App()->GetBanner()->RegenPathAndRedraw();
		}	
	}
	return 0L;
}

int ShapeSelectToolWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	DefWindowProc();
	SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG) COLOR_BTNFACE + 1);
	CreateControls();
	return TRUE;
}

void ShapeSelectToolWnd::OnMove(CPoint pt)
{
	App()->GetSettings()->lastShapeEditToolPos.x = pt.x;
	App()->GetSettings()->lastShapeEditToolPos.y = pt.y;
}

void ShapeSelectToolWnd::OnSize(UINT type, CSize size)
{
	lb.SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOZORDER);
	App()->GetSettings()->lastShapeEditToolSize.x = size.cx;
	App()->GetSettings()->lastShapeEditToolSize.y = size.cy;
}
