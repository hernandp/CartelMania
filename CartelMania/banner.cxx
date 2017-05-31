#include "stdafx.h"
#include "banner.h"
#include "bannerline.h"
#include "TextFx.h"
#include "Geometry.h"
#include "MainWindow.h"
#include "CartelManiaApp.h"

using namespace Gdiplus;
using namespace std;

//
// Default banner values
//

const BannerLayout g_defaultBannerLayout = BannerLayout::SingleLine;
const int g_defaultNumPagesWide = 2;
const int g_defaultNumPagesTall = 1;
const wchar_t* g_defaultShapeName = L"Rectangle";
const wchar_t* g_defaultTopLineText =  L"CartelMania";
const wchar_t* g_defaultBottomLineText = L"Line 2";
const wchar_t* g_defaultFontFamilyName = L"Arial";
const int g_defaultVerticalFill = 50;
const int g_defaultHorizontalFill = 100;
const auto g_defaultVAlign = BannerVerticalAlignment::Center;
const auto g_defaultHAlign = BannerHorizontalAlignment::Center;
const int  g_defaultEasyGlueMarginMm = 15;

Banner::Banner() :
m_layout(g_defaultBannerLayout),
m_pageCountXAxis(g_defaultNumPagesWide),
m_pageCountYAxis(g_defaultNumPagesTall),
m_shapeName(g_defaultShapeName),
m_topLine(make_unique<BannerLine>(g_defaultTopLineText, g_defaultFontFamilyName, FontStyleRegular, make_unique<TextFxSolid>())),
m_bottomLine(make_unique<BannerLine>(g_defaultBottomLineText, g_defaultFontFamilyName, FontStyleRegular, make_unique<TextFxSolid>())),
m_scalePolicy(ScalePolicy::ScaleToFit),
m_verticalFill(g_defaultVerticalFill),
m_horizontalFill(g_defaultHorizontalFill),
m_verticalAlign(g_defaultVAlign),
m_horizontalAlign(g_defaultHAlign),
m_easyGluePrintActive(true),
m_easyGlueMarginVisible(true),
m_easyGlueMarginMm(g_defaultEasyGlueMarginMm)
{	
	
}

Banner::~Banner()
{
}

Gdiplus::RectF Banner::CalcRect(const Gdiplus::RectF& rcClientArea) const
{
	REAL topMargin;
	REAL leftMargin;
	REAL height = rcClientArea.Height * (m_verticalFill / 100.0f);
	REAL width = rcClientArea.Width  * (m_horizontalFill / 100.0f);
	
	switch (m_horizontalAlign)
	{
		case BannerHorizontalAlignment::Center:
			leftMargin = rcClientArea.Width / 2.0f - width / 2.0f;
			break;
		case BannerHorizontalAlignment::Left:
			leftMargin = 0.0f;
			break;
		case BannerHorizontalAlignment::Right:
			leftMargin = rcClientArea.Width - width;
			break;
	}

	switch (m_verticalAlign)
	{
		case BannerVerticalAlignment::Center:
			topMargin = rcClientArea.Height / 2.0f - height / 2.0f;
			break;
		case BannerVerticalAlignment::Top: 
			topMargin = 0.0f;
			break;
		case BannerVerticalAlignment::Bottom: 
			topMargin = rcClientArea.Height - height;
			break;
	}
	
	return RectF(rcClientArea.X + leftMargin, rcClientArea.Y + topMargin, width, height);
}

Gdiplus::RectF Banner::CalcRect(const LPRECT rcClient) const
{
	const RectF rcClientArea((REAL) rcClient->left,
		(REAL) rcClient->top,
		(REAL) (rcClient->right - rcClient->left),
		(REAL) (rcClient->bottom - rcClient->top));

	return CalcRect(rcClientArea);
}

void Banner::SetPageCount(int xAxisPageCount, int yAxisPageCount)
{
	m_pageCountXAxis = xAxisPageCount;
	m_pageCountYAxis = yAxisPageCount;
}

Gdiplus::Size Banner::GetSizeMm() const
{	
	const int printW = App()->GetPrintableAreaMm().Width;
	const int printH = App()->GetPrintableAreaMm().Height;
	const int easyGlueMargin = m_easyGluePrintActive ? m_easyGlueMarginMm : 0;
	return (Gdiplus::Size(
		((m_pageCountXAxis - 1)  * (printW - easyGlueMargin)) +  printW,
		((m_pageCountYAxis - 1)  * (printH - easyGlueMargin)) +  printH));
}

void Banner::PaintOn(HDC hdc, const LPRECT rcClient, int printPageX, int printPageY)
{
	Graphics gr(hdc);
	gr.SetPageUnit(Gdiplus::Unit::UnitPixel);
	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	const RectF rcClientArea((REAL) rcClient->left,
		(REAL) rcClient->top,
		(REAL) (rcClient->right - rcClient->left),
		(REAL) (rcClient->bottom - rcClient->top));

	if (App()->GetSettings()->debugDrawBoundingRects)
		gr.DrawRectangle(&Pen(Color::Blue), rcClientArea);
	
	const RectF bannerRect = CalcRect(rcClientArea);
	REAL leftMargin = bannerRect.X - rcClientArea.X;
	REAL topMargin = bannerRect.Y - rcClientArea.Y;

	// Zoom on the banner area matching the page to print if necessary
	//

	gr.TranslateTransform(bannerRect.X, bannerRect.Y);

	if (printPageX != -1 && printPageY != -1)
	{
		// To do zoom, let p = center of rectangle delimiting the graphics
		// area targeted for the current page.
		// p' = translate p to center of physical page
		// scale to sx/sy factors matching the number of printout pages
		
		const REAL pageWidth = rcClientArea.Width / m_pageCountXAxis;
		const REAL pageHeight = rcClientArea.Height / m_pageCountYAxis;

		const REAL tx = (rcClientArea.Width * ((float) printPageX / m_pageCountXAxis));
		const REAL ty = rcClientArea.Height* ((float) printPageY / m_pageCountYAxis);

		// Take margin into account (if HorzFill != 100%)

		gr.TranslateTransform(-leftMargin, -topMargin);
		gr.ScaleTransform((float) m_pageCountXAxis, (float) m_pageCountYAxis);
		gr.TranslateTransform(leftMargin, topMargin);
		gr.TranslateTransform(-tx, -ty);
	}

	// The banner coordinate space is divided by number of lines, considering the current layout.
	// Each line has it's own local space with x+/y+ pointing left and down in display.

	RectF line1Rect, line2Rect;
	GetLineRects(bannerRect, line1Rect, line2Rect);
	
	m_topLine->DrawOn(gr, line1Rect);

	if (m_layout != BannerLayout::SingleLine)
	{	
		gr.TranslateTransform(0, line1Rect.Height);
		m_bottomLine->DrawOn(gr, line2Rect);
	}		
}

void Banner::GetLineRects(const RectF& bannerRect, RectF& rcLine1, RectF& rcLine2) const
{
	const REAL line1Height = g_proportionTable.at(m_layout).first * bannerRect.Height;
	const REAL line2Height = g_proportionTable.at(m_layout).second * bannerRect.Height;
	
	rcLine1 = RectF(0, 0, bannerRect.Width, line1Height);
	rcLine2 = RectF(0, 0, bannerRect.Width, line2Height);
}

void Banner::Redraw()
{
	InvalidateRect(*App()->GetMainWindow(), NULL, FALSE);
}

void Banner::RegenPathAndRedraw()
{
	m_topLine->InvalidatePath();
	m_bottomLine->InvalidatePath();
	InvalidateRect(*App()->GetMainWindow(), NULL, FALSE);
}
