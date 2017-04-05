#include <windows.h>
#include <gdiplus.h>
#include "textutil.h"
#include "textrend.h"
#include "banner.h"

using namespace Gdiplus;
using namespace std;

void TextFxBlock::Draw(const Banner& banner, Graphics& gr, RectF& rc)
{
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingNone);
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	unique_ptr<Font> fillFont = FindFontToFillTextInRect(gr, rc, banner.GetFont(), banner.GetText1(), format);

	// Cheap block effect

	const int BLOCKDEPTH = 128;

	for (int i = 0; i < BLOCKDEPTH; ++i)
	{
		RectF rcBlock = rc;
		const BYTE layerColorComp = BYTE(255.0f * ((BLOCKDEPTH - i) / REAL(BLOCKDEPTH)));
		rcBlock.Offset(REAL(BLOCKDEPTH - i), REAL(BLOCKDEPTH - i));

		gr.DrawString(banner.GetText1().c_str(), -1, fillFont.get(), rcBlock, &format,
			&SolidBrush(Color(layerColorComp, layerColorComp, layerColorComp)));
	}

	// Draw at the choosen font size
	//

	gr.DrawString(banner.GetText1().c_str(), -1, fillFont.get(), rc, &format,
		&HatchBrush(HatchStyle20Percent, Color::Blue, Color::Red));

}