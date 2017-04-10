#include "banner.h"
#include "textrend.h"
#include <memory.h>
#include <gdiplus.h>

using namespace Gdiplus;
using namespace std;

Banner::Banner() : m_layout(BannerLayout::SingleLine)
{
	// Create the two default lines, cloning Bannermania behavior
	// (for future expansion, we could add more lines)


	BannerLine line1(DEFAULT_LINE1_TEXT, DEFAULT_FONT_NAME, make_unique<TextFxSolid>());
	BannerLine line2(DEFAULT_LINE2_TEXT, DEFAULT_FONT_NAME, make_unique<TextFxSolid>());
	m_lines.push_back(move(line1));
	m_lines.push_back(move(line2));
}

Banner::~Banner()
{
}

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
	// Origin of drawing is set to the upper-left corner of the banner, with X+ pointing down, Y+ pointing left.
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

	// Render text 

	for (const auto& line : m_lines)
		line.DrawOn(gr, bannerRect);
}