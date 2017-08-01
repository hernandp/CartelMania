/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
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
const wchar_t* g_defaultFxName = L"Solid";
const wchar_t* g_defaultShapeName = L"Rectangle";
const wchar_t* g_defaultTopLineText =  L"CartelMania";
const wchar_t* g_defaultBottomLineText = L"Line 2";
const wchar_t* g_defaultFontFamilyName = L"Arial";
const wchar_t* g_defaultColorSchemeName = L"Default";
const int g_defaultVerticalFill = 50;
const int g_defaultHorizontalFill = 100;
const auto g_defaultVAlign = BannerVerticalAlignment::Center;
const auto g_defaultHAlign = BannerHorizontalAlignment::Center;
const int  g_defaultEasyGlueMarginMm = 15;

Banner::Banner() :
m_layout(g_defaultBannerLayout),
m_pageCountXAxis(g_defaultNumPagesWide),
m_pageCountYAxis(g_defaultNumPagesTall),
m_topLine(make_unique<BannerLine>(g_defaultTopLineText, g_defaultTopLineText, g_defaultFontFamilyName, FontStyleRegular, g_defaultFxName)),
m_bottomLine(make_unique<BannerLine>(g_defaultBottomLineText, g_defaultBottomLineText, g_defaultFontFamilyName, FontStyleRegular, g_defaultFxName)),
m_scalePolicy(ScalePolicy::ScaleToFit),
m_verticalFill(g_defaultVerticalFill),
m_horizontalFill(g_defaultHorizontalFill),
m_verticalAlign(g_defaultVAlign),
m_horizontalAlign(g_defaultHAlign),
m_easyGluePrintActive(true),
m_easyGlueMarginVisible(true),
m_easyGlueMarginMm(g_defaultEasyGlueMarginMm),
m_colorSchemeName(g_defaultColorSchemeName)
{	
	
}

Banner::~Banner()
{
}

Gdiplus::RectF Banner::CalcRect(const Gdiplus::RectF& rcClientArea) const
{
	REAL topMargin = 0.0f;
	REAL leftMargin = 0.0f;
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

	bool isPrinting = (printPageX != -1) && (printPageY != -1);

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

	const REAL cpxEasyGlueMargin = m_easyGlueMarginMm / MM_PER_INCH * gr.GetDpiX();
	const REAL cpyEasyGlueMargin = m_easyGlueMarginMm / MM_PER_INCH * gr.GetDpiY();

	if (isPrinting)
	{
		// To do zoom, let p = center of rectangle delimiting the graphics
		// area targeted for the current page.
		// p' = translate p to center of physical page
		// scale to sx/sy factors matching the number of printout pages		
	
		// Calculate the desired view area to focus on

		float offsetEasyGlueX = ((printPageX > 0) ? cpxEasyGlueMargin : 0.0f);
		float offsetEasyGlueY = ((printPageY > 0) ? cpxEasyGlueMargin : 0.0f);
		float pctX = ((float) printPageX / m_pageCountXAxis);
		float pctY = ((float) printPageY / m_pageCountYAxis);

		const float sx = (float) GetSizeMm().Width / (App()->GetPrintableAreaMm().Width * m_pageCountXAxis);
		const float sy = (float) GetSizeMm().Height / (App()->GetPrintableAreaMm().Height * m_pageCountYAxis);		
		const float tx = printPageX * (-rcClientArea.Width + cpxEasyGlueMargin);
		const float ty = printPageY * (-rcClientArea.Height + cpyEasyGlueMargin);

		gr.TranslateTransform(-leftMargin, -topMargin);
		gr.TranslateTransform(tx, ty);
		gr.ScaleTransform(m_pageCountXAxis * sx, m_pageCountYAxis * sy);
		gr.TranslateTransform(leftMargin, topMargin);
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

	// Draw EasyGlue Margin, if visible and active

	if (isPrinting && m_easyGluePrintActive && m_easyGlueMarginVisible)
	{
		gr.ResetTransform();

		Pen pen(Color::Black, 0.25f / MM_PER_INCH * gr.GetDpiY());
		pen.SetDashStyle(DashStyleDashDotDot);

		if (printPageX < m_pageCountXAxis - 1)
		{
			gr.DrawLine(&pen, rcClientArea.Width - cpxEasyGlueMargin, 0.0f, rcClientArea.Width - cpxEasyGlueMargin, rcClientArea.Height);
		}

		if (printPageY < m_pageCountYAxis - 1)
		{
			gr.DrawLine(&pen, 0.0f, rcClientArea.Height - cpyEasyGlueMargin, rcClientArea.Width, rcClientArea.Height - cpyEasyGlueMargin);
		}
	}
}

void Banner::GetLineRects(const RectF& bannerRect, RectF& rcLine1, RectF& rcLine2, 
						  bool useClientWindowCoordinates) const
{
	const REAL line1Height = g_proportionTable.at(m_layout).first * bannerRect.Height;
	const REAL line2Height = g_proportionTable.at(m_layout).second * bannerRect.Height;
	
	if (useClientWindowCoordinates)
	{
		rcLine1 = RectF(bannerRect.X, 
			bannerRect.Y, 
			bannerRect.Width, 
			line1Height);
		rcLine2 = RectF(bannerRect.X, 
			bannerRect.Y + line1Height + 1, 
			bannerRect.Width, 
			line2Height);
	}
	else
	{
		rcLine1 = RectF(0, 0, bannerRect.Width, line1Height);
		rcLine2 = RectF(0, 0, bannerRect.Width, line2Height);
	}
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

bool Banner::Serialize(const std::wstring & file) const
{
	//
	// I18N Warning: We always store English names from shape and other tables
	//

	pugi::xml_document doc;
	auto root = doc.append_child(L"CartelManiaFile");
	root.append_attribute(L"version").set_value(L"1");

	auto lineLayoutNode = root.append_child(L"LineLayout");
	lineLayoutNode.append_attribute(L"type").set_value((int) m_layout);

	auto pageLayoutNode = root.append_child(L"PageLayout");
	pageLayoutNode.append_attribute(L"PageCountX").set_value(m_pageCountXAxis);
	pageLayoutNode.append_attribute(L"PageCountY").set_value(m_pageCountYAxis);
	pageLayoutNode.append_attribute(L"VFill").set_value(m_verticalFill);
	pageLayoutNode.append_attribute(L"HFill").set_value(m_horizontalFill);
	pageLayoutNode.append_attribute(L"VAlign").set_value((int) m_verticalAlign);
	pageLayoutNode.append_attribute(L"HAlign").set_value((int) m_horizontalAlign);
	pageLayoutNode.append_attribute(L"EasyGlueEnable").set_value(m_easyGluePrintActive);
	pageLayoutNode.append_attribute(L"EasyGlueVisible").set_value(m_easyGlueMarginVisible);
	pageLayoutNode.append_attribute(L"EasyGlueMarginMm").set_value(m_easyGlueMarginMm);

	SerializeBannerLine(root, L"TopLine", *m_topLine);
	SerializeBannerLine(root, L"BottomLine", *m_bottomLine);
	
	if (!doc.save_file(file.c_str()))
	{
		return false;
	}

	return true;
}

void Banner::SerializeBannerLine(pugi::xml_node &root, const std::wstring nodeName, const BannerLine& bannerLine) const
{
	auto lineNode = root.append_child(nodeName.c_str());
	lineNode.append_child(L"Text").append_child(pugi::node_pcdata).set_value(bannerLine.GetText().c_str());
	lineNode.append_child(L"Font").append_child(pugi::node_pcdata).set_value(bannerLine.GetFontName().c_str());
	lineNode.append_attribute(L"Shape").set_value(bannerLine.GetShapeName().c_str());
	auto fxNode = lineNode.append_child(L"Effect");
	fxNode.append_attribute(L"Name").set_value(bannerLine.GetTextFxName().c_str());

	for (int i = 0; i < bannerLine.GetTextFx()->GetColorPropertyCount(); ++i)
	{
		auto colorPropName = bannerLine.GetTextFx()->GetColorPropertyItem(i).GetClassString();
		auto colorValueName = bannerLine.GetTextFx()->GetColorPropertyItem(i).GetValue();
		auto colorPropNode = fxNode.append_child(L"ColorProperty");
		colorPropNode.append_attribute(L"class").set_value(colorPropName);
		colorPropNode.append_attribute(L"value").set_value(colorValueName.c_str());
	}
}

void Banner::DeserializeBannerLine(pugi::xml_node &root, const std::wstring nodeName, BannerLine& bannerLine)
{
	auto lineNode = root.child(nodeName.c_str());
	auto shape = lineNode.attribute(L"Shape");
	auto text = lineNode.child(L"Text").text();
	auto font = lineNode.child(L"Font").text();

	auto effectNode = lineNode.child(L"Effect");
	auto effectName = effectNode.attribute(L"Name");

	for (auto& fxNode : effectNode.children())
	{
		auto fxClass = fxNode.attribute(L"class");
		auto fxClassValue = fxNode.attribute(L"value");
	}
}

bool Banner::Deserialize(const std::wstring& file, ptrdiff_t& error_offset)
{
	pugi::xml_document doc;
	auto result = doc.load_file(file.c_str());

	if (result.status != pugi::status_ok)
	{
		error_offset = result.offset;
		return false;
	}

	auto root = doc.child(L"CartelManiaFile");
	if (root.empty() || root.attribute(L"version").as_int() != 1)
	{
		return false;
	}

	auto lineLayoutNode = root.child(L"LineLayout");
	auto layout = lineLayoutNode.attribute(L"type").as_int();

	auto pageLayoutNode = root.child(L"PageLayout");
	auto pageCountX = pageLayoutNode.attribute(L"PageCountX").as_int(g_defaultNumPagesWide);
	auto pageCountY = pageLayoutNode.attribute(L"PageCountY").as_int(g_defaultNumPagesTall);
	auto vFill = pageLayoutNode.attribute(L"VFill").as_int(g_defaultVerticalFill);
	auto hFill = pageLayoutNode.attribute(L"HFill").as_int(g_defaultHorizontalFill);
	auto vAlign = pageLayoutNode.attribute(L"VAlign").as_int((int) g_defaultVAlign);
	auto hAlign = pageLayoutNode.attribute(L"HAlign").as_int((int) g_defaultHAlign);
	auto easyGlueActive = pageLayoutNode.attribute(L"EasyGlueEnable").as_bool(true);
	auto easyGlueVisible = pageLayoutNode.attribute(L"EasyGlueVisible").as_bool(true);
	auto easyGlueMarginMm = pageLayoutNode.attribute(L"EasyGlueMarginMm").as_int(g_defaultEasyGlueMarginMm);
	
	DeserializeBannerLine(root, L"TopLine", *m_topLine);
	DeserializeBannerLine(root, L"BottomLine", *m_bottomLine);
		
	return true;
}