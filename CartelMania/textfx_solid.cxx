#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>
#include "textrend.h"
#include "textutil.h"
#include "banner.h"

using namespace Gdiplus;
using namespace std;

void TextFxSolid::Draw(const Banner& banner, Graphics& gr, RectF& rc)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingNone);
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	REAL fontSize;
	FontFamily family(banner.GetFont().c_str());
	unique_ptr<Font> font = FindFontToFillTextInRect(gr, rc, family, banner.GetText1(), format, &fontSize);
	
	GraphicsPath path;
	Pen pen(Color::Black, 2); // should get from current Colorset
	HatchBrush brush(HatchStyle25Percent, Color::White, Color::Red);
	path.AddString(banner.GetText1().c_str(), -1, &family, FontStyleRegular, fontSize, rc, &format);
	gr.FillPath(&brush, &path);
	gr.DrawPath(&pen, &path);

}