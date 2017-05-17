#include "stdafx.h"
#include "banner.h"
#include "bannerline.h"
#include "TextFx.h"
#include "Geometry.h"
#include "MainWindow.h"
#include "CartelManiaApp.h"

using namespace Gdiplus;
using namespace std;

//----------------------------------------------------------------------------

Banner::Banner() :
m_layout(BannerLayout::SingleLine),
m_size(0,0),
m_shapeName(L"Rectangle"),
m_topLine(make_unique<BannerLine>(DEFAULT_TOPLINE_TEXT, DEFAULT_FONT_NAME, FontStyleRegular, make_unique<TextFxSolid>())),
m_bottomLine(make_unique<BannerLine>(DEFAULT_BOTTOMLINE_TEXT, DEFAULT_FONT_NAME, FontStyleRegular, make_unique<TextFxSolid>())),
m_scalePolicy(ScalePolicy::ScaleToFit),
m_verticalFill(50),
m_horizontalFill(90),
m_verticalAlign(BannerVerticalAlignment::Center),
m_horizontalAlign(BannerHorizontalAlignment::Center)
{	
	
}
//----------------------------------------------------------------------------

Banner::~Banner()
{
}

Gdiplus::RectF Banner::GetRect(const Gdiplus::RectF& rcClientArea) const
{
	return RectF(0, 0, rcClientArea.Width - BANNER_MARGIN_PX,
		rcClientArea.Height * BANNER_HEIGHT_PCT / 100.0f);
}


Gdiplus::RectF Banner::GetRect(const LPRECT rcClient) const
{
	const RectF rcClientArea((REAL) rcClient->left,
		(REAL) rcClient->top,
		(REAL) (rcClient->right - rcClient->left),
		(REAL) (rcClient->bottom - rcClient->top));

	return GetRect(rcClientArea);
}

//----------------------------------------------------------------------------
void Banner::PaintOn(HDC hdc, const LPRECT rcClient)
{
	Graphics gr(hdc);
	gr.SetPageUnit(Gdiplus::Unit::UnitPixel);
	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	const RectF rcClientArea((REAL) rcClient->left,
		(REAL) rcClient->top,
		(REAL) (rcClient->right - rcClient->left),
		(REAL) (rcClient->bottom - rcClient->top));

	gr.DrawRectangle(&Pen(Color::Blue), rcClientArea);

	const RectF bannerRect = GetRect(rcClientArea);

	// The banner coordinate space is divided by number of lines, considering the current layout.
	// Each line has it's own local space with x+/y+ pointing left and down in display.

	gr.TranslateTransform(rcClientArea.X + BANNER_MARGIN_PX / 2, rcClientArea.Y + rcClientArea.Height / 2.0f - bannerRect.Height / 2.0f);

	RectF line1Rect, line2Rect;
	GetLineRects(bannerRect, line1Rect, line2Rect);
	
	m_topLine->DrawOn(gr, line1Rect);

	if (m_layout != BannerLayout::SingleLine)
	{	
		gr.TranslateTransform(0, line1Rect.Height);
		m_bottomLine->DrawOn(gr, line2Rect);
	}		
}

inline Gdiplus::Size Banner::CalcPrintOutputPageCount(const Gdiplus::Size & paperSize)
{
	return Size((int) ceilf((float)paperSize.Height / m_size.Height), 
		(int) ceilf((float)paperSize.Width / m_size.Width));
}

void Banner::GetLineRects(const RectF& bannerRect, RectF& rcLine1, RectF& rcLine2) const
{
	const REAL line1Height = g_proportionTable.at(m_layout).first * bannerRect.Height;
	const REAL line2Height = g_proportionTable.at(m_layout).second * bannerRect.Height;
	
	rcLine1 = RectF(0, 0, bannerRect.Width, line1Height);
	rcLine2 = RectF(0, 0, bannerRect.Width, line2Height);
}

void Banner::BuildPaths()
{
	/*GraphicsPath path1, path2;
	path1.AddString(m_lines[0].)*/
}

void Banner::Invalidate()
{
	m_topLine->InvalidatePath();
	m_bottomLine->InvalidatePath();
	InvalidateRect(*App()->GetMainWindow(), NULL, FALSE);
}

//----------------------------------------------------------------------------
