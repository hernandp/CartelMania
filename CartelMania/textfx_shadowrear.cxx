#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>
#include "textrend.h"
#include "textutil.h"
#include "banner.h"

using namespace Gdiplus;
using namespace std;
using namespace Cartelmania;

void TextFxShadowRear::DrawLine(const BannerLine& line, Graphics& gr, const RectF& bannerRect)
{
	StringFormat format;
	format.SetTrimming(StringTrimmingNone);
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	RectF boundingBox;
	FontFamily family(line.GetFontName().c_str());
	unique_ptr<Font> font = FindFontToFillTextInRect(gr, bannerRect, family, line.GetText(), format, &boundingBox);

	RectF rcBounds = QueryStringPathBounds(gr, line.GetText(), *font, bannerRect, format);
		
	// Shear matrix by K units yields for (x,y):
	// x' = x + Ky
	// y' = y + Kx
	//
	// K is the tangent of the shearing angle (so K=1 -> 45 deg)
	
	
	Matrix mtx;
	mtx.Shear(-0.2f, 0.0f);
	//mtx.Scale(1.0f, -1.0f);
	//mtx.Translate(-0.2f, 0.0f);	

	rcBounds.X = rcBounds.Y = 0;

	/*GraphicsPath p;
	p.AddRectangle(rcBounds);
	p.Transform(&mtx);
	gr.FillPath(&SolidBrush(Gdiplus::Color::Red), &p);
	gr.DrawRectangle(&Pen(Gdiplus::Color::Black), rcBounds);
	
	DrawStringPath(gr, g_Colors[0], banner.GetText1(), rcBounds, format, *font.get(), &mtx);
	DrawStringPath(gr, g_Colors[2], banner.GetText1(), boundingBox, format, *font.get());*/
}