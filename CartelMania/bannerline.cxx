#include "stdafx.h"
#include "bannerline.h"
#include "TextFx.h"
#include "Geometry.h"
#include "CartelManiaApp.h"

using namespace Gdiplus;
using namespace std;

BannerLine::BannerLine(
	const wstring& defaultText,
	const wstring& text,
	const wstring& fontName, 
	FontStyle fontStyle,
	TextFx* effect) :
	m_textFx(effect),
	m_defaultText(defaultText),
	m_text(text),
	m_fontName(fontName),
	m_fontStyle(fontStyle),
	m_needRegen(true)
{

}

void BannerLine::SetTextFx(TextFx* newFx)
{
	m_textFx = newFx;
}

void BannerLine::DrawOn(Graphics & gr, const RectF & rect) 
{	
// Random background for debugging purposes!
/*
	gr.FillRectangle(&HatchBrush(Gdiplus::HatchStyle50Percent,
		Color(rand() % 255, rand() % 255, rand() % 255),
		Color(rand() % 255, rand() % 255, rand() % 255)), rect);*/

	// Lazy initialization
	if (m_textFx == nullptr)
	{
		m_textFx = App()->GetEffectTable()->LookupName(L"Solid").get();
	}
	

	m_textFx->DrawLine(*this, gr, rect);
}

GraphicsPath* BannerLine::GetPath()
{
	if (m_needRegen)
		BuildPath();

	return m_path.get();	
}

unique_ptr<Gdiplus::GraphicsPath> BannerLine::GetPathCopy()
{
	return unique_ptr<Gdiplus::GraphicsPath>(GetPath()->Clone());
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

void BannerLine::SetText(const std::wstring& text)
{
	m_text = text;
}
