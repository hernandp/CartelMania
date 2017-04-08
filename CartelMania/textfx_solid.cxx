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

void TextFxSolid::Draw(const Banner& banner, Graphics& gr, const RectF& bannerRect)
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

	DrawStringInfo di;
	di.gr				= &gr;
	di.text				= banner.GetText1();
	di.rect				= &boundingBox;
	di.stringFormat		= &format;
	di.faceColor		= g_Colors[2];
	di.outlineColor		= g_Colors[3];
	di.outlineWidth		= m_outlineWidth;
	di.font				= font.get();
	di.xForm			= &mtx;

	DrawStringPath(di);
}