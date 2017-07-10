/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
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
	const wstring& effect) :
	m_defaultText(defaultText),
	m_text(text),
	m_fontName(fontName),
	m_fontStyle(fontStyle),
	m_shapeName(L"Rectangle"),
	m_needRegen(true)
{
	SetTextFx(effect);
}

void BannerLine::SetTextFx(const std::wstring& fxName)
{
	m_textFxName = fxName;
	m_textFx = App()->GetEffectTable()->LookupName(fxName)();
}

void BannerLine::DrawOn(Graphics & gr, const RectF & rect) 
{	
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
