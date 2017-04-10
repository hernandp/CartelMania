#include "bannerline.h"
#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;
using namespace std;

BannerLine::BannerLine(const wstring & text,
	const wstring & fontName, 
	unique_ptr<TextFXRenderer> effect) :
	m_textFx(std::move(effect)),
	m_text(text),
	m_fontName(fontName)
{
}

void BannerLine::SetTextFx(unique_ptr<TextFXRenderer> newFx)
{
	m_textFx = move(newFx);
}

void BannerLine::DrawOn(Graphics & gr, const RectF & rect) const
{	
// Random background for debugging purposes!

	gr.FillRectangle(&HatchBrush(Gdiplus::HatchStyle50Percent,
		Color(rand() % 255, rand() % 255, rand() % 255),
		Color(rand() % 255, rand() % 255, rand() % 255)), rect);

	m_textFx->DrawLine(*this, gr, rect);
}
