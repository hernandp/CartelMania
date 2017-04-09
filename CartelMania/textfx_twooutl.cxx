#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>
#include "textrend.h"
#include "textutil.h"
#include "banner.h"
#include "colors.h"

using namespace Gdiplus;
using namespace std;
using namespace Cartelmania;

void TextFxTwoOutlines::Draw(const Banner& banner, Graphics& gr, const RectF& bannerRect)
{
	StringFormat format;
	format.SetTrimming(StringTrimmingNone);
	format.SetFormatFlags(StringFormatFlagsNoWrap);
	format.SetAlignment(StringAlignmentCenter);

	RectF boundingBox;
	FontFamily family(banner.GetFont().c_str());
	unique_ptr<Font> font = FindFontToFillTextInRect(gr, bannerRect, family, banner.GetText1(), format, &boundingBox);

	// Center vertically [ user could choose other alignments ]
	//
	Matrix mtx;
	mtx.Translate(0.0f, (bannerRect.Height / 2) - (boundingBox.Height / 2));

	auto fontSize = PointToPixels(gr, font->GetSize());
	auto fontStyle = font->GetStyle();
	FontFamily fontFamily;
	font->GetFamily(&fontFamily);

	GraphicsPath path;
	Pen penIn(Gdiplus::Color::Yellow, m_outlineWidth);
	Pen penOut(Gdiplus::Color::Blue, m_outlineWidth * 2);
	HatchBrush brush(g_Colors[2].m_hatch, g_Colors[2].m_fg, g_Colors[2].m_bg);
	path.AddString(banner.GetText1().c_str(), -1, &fontFamily, fontStyle, fontSize, boundingBox, &format);
	path.Transform(&mtx);

	
	gr.DrawPath(&penOut, &path);
	gr.DrawPath(&penIn, &path);
	gr.FillPath(&brush, &path);
}