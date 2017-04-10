#include <windows.h>
#include <gdiplus.h>
#include "textutil.h"
#include "textrend.h"
#include "banner.h"

using namespace Gdiplus;
using namespace std;
using namespace Cartelmania;

void TextFxBlock::DrawLine(const BannerLine& line, Graphics& gr, const RectF& rc)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingNone);
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	FontFamily family(line.GetFontName().c_str());
	unique_ptr<Font> fillFont = FindFontToFillTextInRect(gr, rc, family, line.GetText(), format);

	// Cheap block effect

	const int BLOCKDEPTH = 128;

	for (int i = 0; i < BLOCKDEPTH; ++i)
	{
		RectF rcBlock = rc;
		const BYTE layerColorComp = BYTE(255.0f * ((BLOCKDEPTH - i) / REAL(BLOCKDEPTH)));
		rcBlock.Offset(REAL(BLOCKDEPTH - i), REAL(BLOCKDEPTH - i));

		gr.DrawString(line.GetText().c_str(), -1, fillFont.get(), rcBlock, &format,
			&SolidBrush(Gdiplus::Color(layerColorComp, layerColorComp, layerColorComp)));
	}

	// Draw at the choosen font size
	//

	gr.DrawString(line.GetText().c_str(), -1, fillFont.get(), rc, &format,
		&HatchBrush(HatchStyle20Percent, Gdiplus::Color::Blue, Gdiplus::Color::Red));
}