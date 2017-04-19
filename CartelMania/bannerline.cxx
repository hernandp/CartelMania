#include "bannerline.h"
#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>
#include "geom.h"

using namespace Gdiplus;
using namespace std;

BannerLine::BannerLine(const wstring & text,
	const wstring & fontName, 
	FontStyle fontStyle,
	unique_ptr<TextFXRenderer> effect) :
	m_textFx(std::move(effect)),
	m_text(text),
	m_fontName(fontName),
	m_fontStyle(fontStyle),
	m_needRegen(true)
{

}

void BannerLine::SetTextFx(unique_ptr<TextFXRenderer> newFx)
{
	m_textFx = move(newFx);
}

void BannerLine::DrawOn(Graphics & gr, const RectF & rect) 
{	
// Random background for debugging purposes!
/*
	gr.FillRectangle(&HatchBrush(Gdiplus::HatchStyle50Percent,
		Color(rand() % 255, rand() % 255, rand() % 255),
		Color(rand() % 255, rand() % 255, rand() % 255)), rect);*/

	m_textFx->DrawLine(*this, gr, rect);
}

GraphicsPath* BannerLine::GetPath()
{
	if (m_needRegen)
		BuildPath();

	return m_path.get();	
}

void BannerLine::BuildPath()
{
	StringFormat strFormat;
	strFormat.SetTrimming(StringTrimmingNone);
	strFormat.SetFormatFlags(StringFormatFlagsNoWrap);

	GraphicsPath path;
	path.AddString(m_text.c_str(), -1, &FontFamily(m_fontName.c_str()), m_fontStyle, 256, Point(0,0), &strFormat);
	path.Flatten();
	auto newPath = make_unique<GraphicsPath>();
	SubdividePath(path, *newPath);
	m_path = move(newPath);

	m_needRegen = false;
}
