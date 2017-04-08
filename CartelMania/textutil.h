#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>
#include "colors.h"

namespace Cartelmania
{
//----------------------------------------------------------------------------
//
// Structure and type definitions
//
//----------------------------------------------------------------------------
struct DrawStringInfo
{
	Gdiplus::Graphics*		gr				= nullptr;
	Color 					faceColor;
	Color 					outlineColor;
	float					outlineWidth;
	std::wstring 			text;
	Gdiplus::RectF*			rect			= nullptr;
	Gdiplus::StringFormat*	stringFormat	= nullptr;
	Gdiplus::Font*			font			= nullptr;
	Gdiplus::Matrix*		xForm			= nullptr;
};

//----------------------------------------------------------------------------
//
// Functions to assist in calculation, approximations, not involved directly
// in drawing text.
//
//----------------------------------------------------------------------------

Gdiplus::REAL PointToPixels(const Gdiplus::Graphics& gr,
	Gdiplus::REAL pointSize);

//----------------------------------------------------------------------------

std::unique_ptr<Gdiplus::Font> FindFontToFillTextInRect(
	const Gdiplus::Graphics& gr,
	const Gdiplus::RectF& rc,
	const Gdiplus::FontFamily& fontFamily,
	const std::wstring& text,
	const Gdiplus::StringFormat& format,
	OUT Gdiplus::RectF* rcBound = nullptr);


//----------------------------------------------------------------------------

Gdiplus::RectF QueryStringPathBounds(
	const Gdiplus::Graphics& gr,
	const std::wstring& text,
	const Gdiplus::Font& font,
	const Gdiplus::RectF& rc,
	const Gdiplus::StringFormat& format);

//----------------------------------------------------------------------------
//
// Functions for drawing common objects in a surface
//
//----------------------------------------------------------------------------
void DrawStringPath(const DrawStringInfo& di);

//----------------------------------------------------------------------------

void DrawStringPathShear(Gdiplus::Graphics& gr,
	const Color& color,
	const std::wstring& text,
	const Gdiplus::RectF& rc,
	const Gdiplus::StringFormat& format,
	const Gdiplus::Font& font,
	const Gdiplus::PointF& shear,
	const Gdiplus::Matrix* xForm = nullptr);

//----------------------------------------------------------------------------
}