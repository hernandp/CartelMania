#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>
#include "textrend.h"
#include "textutil.h"
#include "banner.h"
#include "colors.h"
#include "Debug.h"

using namespace Gdiplus;
using namespace std;
using namespace Cartelmania;

void TextFxSolid::DrawLine(const BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	StringFormat strFormat;
	strFormat.SetTrimming(StringTrimmingNone);
	strFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	strFormat.SetAlignment(StringAlignmentCenter);

	RectF boundingBox;
	FontFamily family(line.GetFontName().c_str());
	unique_ptr<Font> font = FindFontToFillTextInRect(gr, lineRect, family, line.GetText(), strFormat, &boundingBox);

	auto fontSize = PointToPixels(gr, font->GetSize());
	auto fontStyle = font->GetStyle();
	Gdiplus::FontFamily fontFamily;
	font->GetFamily(&fontFamily);

	Gdiplus::GraphicsPath path;
	Gdiplus::Pen pen(g_Colors[3].m_fg, m_outlineWidth);
	Gdiplus::HatchBrush brush(g_Colors[2].m_hatch, g_Colors[2].m_fg, g_Colors[2].m_bg);
	path.AddString(line.GetText().c_str(), -1, &fontFamily, fontStyle, fontSize, lineRect, &strFormat);

	// The measured rect passed to AddString accounts for font descent, even if all chars in the string
	// are upon the baseline. We scale the enclosing bounding box of the path to fill the drawing area,
	// minus some margin.
	//

	// Align at center

	RectF bounds;
	path.GetBounds(&bounds, nullptr, &pen);
	Matrix vAlign;
	vAlign.Translate(0, (lineRect.Height/2 - bounds.Height/2) - bounds.Y);
	path.Transform(&vAlign);

	XASSERT(lineRect.Contains(bounds));

	// Calculate how much X/Y-scaling we need.
	// Mantain aspect-ratio (sX=sY)
	
	const REAL ratio = bounds.Width / bounds.Height;
	const REAL s = lineRect.Height / bounds.Height;

	Matrix mtx;
	mtx.Translate(lineRect.Width / 2.0f, lineRect.Height / 2.0f);
	mtx.Scale(s, s);
	mtx.Translate(-lineRect.Width / 2.0f, -lineRect.Height / 2.0f);
	path.Transform(&mtx);

	gr.FillPath(&brush, &path);
	gr.DrawPath(&pen, &path);
}