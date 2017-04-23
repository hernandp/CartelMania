#include "stdafx.h"
#include "banner.h"
#include "bannerline.h"
#include "textrend.h"
#include "geom.h"

using namespace Gdiplus;
using namespace std;

//----------------------------------------------------------------------------

Banner::Banner() : m_layout(BannerLayout::SingleLine)
{
	// Create the two default lines, cloning Bannermania behavior

	m_topLine    = make_unique<BannerLine>(DEFAULT_TOPLINE_TEXT, DEFAULT_FONT_NAME, FontStyleRegular, make_unique<TextFxSolid>());
	m_bottomLine = make_unique<BannerLine>(DEFAULT_BOTTOMLINE_TEXT, DEFAULT_FONT_NAME, FontStyleRegular, make_unique<TextFxSolid>());
}
//----------------------------------------------------------------------------

Banner::~Banner()
{
}

//----------------------------------------------------------------------------
void Banner::PaintOn(HDC hdc, const LPRECT rcClient)
{
	Graphics gr(hdc);
	
	const RectF rcClientArea((REAL) rcClient->left, 
		(REAL) rcClient->top, 
		(REAL) (rcClient->right - rcClient->left),
		(REAL) (rcClient->bottom - rcClient->top));

	// Draw client window background
	//
	gr.FillRectangle(&HatchBrush(HatchStyle10Percent, Color::Blue, Color::DarkCyan), rcClientArea);
	
	// Transform coordinate space.
	// Banner area spans over 100% width minus borders, and 50% height, of the window client area.
	// Origin of drawing is set to the upper-left corner of the banner, with Y+ pointing down, X+ pointing left.
	// We still use pixel units, however.
	//
	const RectF bannerRect(0, 0, rcClientArea.Width - BANNER_MARGIN_PX, 
		rcClientArea.Height * BANNER_HEIGHT_PCT / 100.0f);
	
	gr.TranslateTransform(rcClientArea.X + BANNER_MARGIN_PX / 2, rcClientArea.Height / 2.0f - bannerRect.Height / 2.0f);
	gr.FillRectangle(&SolidBrush(Color::Black), bannerRect);

	// Draw banner page and shadow
	
	RectF bannerShadowRect = bannerRect;
	bannerShadowRect.Offset(4.0f, 4.0f);
	gr.FillRectangle(&SolidBrush(Color::Black), bannerShadowRect);
	gr.FillRectangle(&SolidBrush(Color::White), bannerRect);
	gr.DrawRectangle(&Pen(Color::Black), bannerRect);


	// Step1: Build paths from the text lines

	const GraphicsPath* gp0 = m_topLine->GetPath();
	const GraphicsPath* gp1 = m_bottomLine->GetPath();
	
	// The banner coordinate space is divided by number of lines, considering the current layout.
	// Each line has it's own local space with x+/y+ pointing left and down in display.
	
	const REAL line1Height = g_proportionTable.at(m_layout).first * bannerRect.Height;
	const RectF line1Rect(0, 0, bannerRect.Width, line1Height);

	m_topLine->DrawOn(gr, line1Rect);

	if (m_layout != BannerLayout::SingleLine)
	{
		// Render second line

		const REAL line2Height = g_proportionTable.at(m_layout).second * bannerRect.Height;
		const RectF line2Rect(0, 0, bannerRect.Width, line2Height);

		gr.TranslateTransform(0, line1Rect.Height);
		m_bottomLine->DrawOn(gr, line2Rect);
	}	
}

void Banner::BuildPaths()
{
	/*GraphicsPath path1, path2;
	path1.AddString(m_lines[0].)*/
}

//----------------------------------------------------------------------------
