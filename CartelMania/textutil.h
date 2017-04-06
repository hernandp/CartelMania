#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>

inline std::unique_ptr<Gdiplus::Font> FindFontToFillTextInRect(
	const Gdiplus::Graphics& gr,
	const Gdiplus::RectF& rc,
	const Gdiplus::FontFamily& fontFamily,
	const std::wstring& text,
	const Gdiplus::StringFormat& format,
	OUT Gdiplus::REAL* outFontSize = nullptr)
{
	// This iterative approximation method is a *total Hack* 
	// better solution? 

	Gdiplus::RectF boundingBox{};
	Gdiplus::REAL fontSize = 10.0f;
	while (boundingBox.Height < rc.Height && boundingBox.Width < rc.Width)
	{
		fontSize += 1.0f;
		Gdiplus::Font tryFont(&fontFamily, fontSize, 0, Gdiplus::Unit::UnitPoint);
		gr.MeasureString(text.c_str(), -1, &tryFont, rc, &format, &boundingBox);
	}

	if (outFontSize)
		*outFontSize = fontSize;

	return std::make_unique<Gdiplus::Font>(&fontFamily, fontSize, 0, Gdiplus::Unit::UnitPoint);
}
