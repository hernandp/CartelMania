#include "stdafx.h"
//#include "textutil.h"
//#include <cassert>
///*
////----------------------------------------------------------------------------
//
//Gdiplus::REAL Cartelmania::PointToPixels(const Gdiplus::Graphics& gr, Gdiplus::REAL pointSize)
//{
//	return gr.GetDpiY() * pointSize / 72.0f;
//}
//
////----------------------------------------------------------------------------
//
//std::unique_ptr<Gdiplus::Font> Cartelmania::FindFontToFillTextInRect(const Gdiplus::Graphics& gr, 
//	const Gdiplus::RectF& rc, 
//	const Gdiplus::FontFamily& fontFamily, 
//	const std::wstring& text,
//	const Gdiplus::StringFormat& format,
//	OUT Gdiplus::RectF* rcBound /*= nullptr*/)
//{
//	// This iterative approximation method is a *total Hack* 
//	// better solution? 
//
//	Gdiplus::RectF boundingBox{};
//	Gdiplus::REAL fontSize{ 10.0f };
//	const auto textLen = text.size();
//	while (boundingBox.Height < rc.Height && boundingBox.Width < rc.Width)
//	{
//		Gdiplus::Font tryFont(&fontFamily, fontSize, 0, Gdiplus::Unit::UnitPoint);
//		gr.MeasureString(text.c_str(), int(textLen), &tryFont, rc, &format, &boundingBox);
//		fontSize += 1.0f;
//	}
//
//	if (rcBound)
//		*rcBound = boundingBox;
//	return std::make_unique<Gdiplus::Font>(&fontFamily, fontSize, 0, Gdiplus::Unit::UnitPoint);
//}
//
////----------------------------------------------------------------------------
//
//Gdiplus::RectF Cartelmania::QueryStringPathBounds(const Gdiplus::Graphics& gr,
//	const std::wstring& text, 
//	const Gdiplus::Font& font, 
//	const Gdiplus::RectF& rc, 
//	const Gdiplus::StringFormat& format)
//{
//	Gdiplus::FontFamily family;
//	Gdiplus::GraphicsPath path;
//	Gdiplus::RectF rcBounds;
//
//	auto fontSize = PointToPixels(gr, font.GetSize());
//	font.GetFamily(&family);
//	path.AddString(text.c_str(), -1, &family, font.GetStyle(), fontSize, rc, &format);
//	path.GetBounds(&rcBounds);
//	return rcBounds;
//}
//
////----------------------------------------------------------------------------
//
//void Cartelmania::DrawStringPath(const DrawStringInfo& di)
//{
//	assert(di.gr);
//	assert(di.font);
//	assert(di.rect);
//	assert(di.stringFormat);
//	
//	auto fontSize = PointToPixels(*di.gr, di.font->GetSize());
//	auto fontStyle = di.font->GetStyle();
//	Gdiplus::FontFamily fontFamily;
//	di.font->GetFamily(&fontFamily);
//
//	Gdiplus::GraphicsPath path;
//	Gdiplus::Pen pen(di.outlineColor.m_fg, di.outlineWidth); 
//	Gdiplus::HatchBrush brush(di.faceColor.m_hatch, di.faceColor.m_fg, di.faceColor.m_bg);
//	path.AddString(di.text.c_str(), -1, &fontFamily, fontStyle, fontSize, *di.rect, di.stringFormat);
//
//	if (di.xForm)
//		path.Transform(di.xForm);
//
//	di.gr->FillPath(&brush, &path);
//	di.gr->DrawPath(&pen, &path);
//}
//
////----------------------------------------------------------------------------
//
//void Cartelmania::DrawStringPathShear(Gdiplus::Graphics& gr, 
//	const Color& color, 
//	const std::wstring& text, 
//	const Gdiplus::RectF& rc, 
//	const Gdiplus::StringFormat& format, 
//	const Gdiplus::Font& font, 
//	const Gdiplus::PointF& shear,
//	const Gdiplus::Matrix* xForm /*= nullptr*/)
//{
//	auto fontSize = PointToPixels(gr, font.GetSize());
//	auto fontStyle = font.GetStyle();
//	Gdiplus::FontFamily fontFamily;
//	font.GetFamily(&fontFamily);
//
//	Gdiplus::GraphicsPath path;
//	Gdiplus::Pen pen(Gdiplus::Color::Black, 2); // should get from current Colorset
//	Gdiplus::HatchBrush brush(color.m_hatch, color.m_fg, color.m_bg);
//	path.AddString(text.c_str(), -1, &fontFamily, fontStyle, fontSize, rc, &format);
//
//	if (xForm)
//		path.Transform(xForm);
//	
//
//	gr.FillPath(&brush, &path);
//	gr.DrawPath(&pen, &path);
//}
//
////----------------------------------------------------------------------------