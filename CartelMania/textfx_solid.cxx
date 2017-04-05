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

	unique_ptr<Font> font = FindFontToFillTextInRect(gr, rc, banner.GetFont(), banner.GetText1(), format);
	
	gr.DrawString(banner.GetText1().c_str(), -1, font.get(), rc, &format, &SolidBrush(Color::Black));

}