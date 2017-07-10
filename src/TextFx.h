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
#pragma once
#include "ColorProps.h"
#include "ColorTable.h"

//----------------------------------------------------------------------------
//
// Forward declarations
//
//----------------------------------------------------------------------------

class Banner;
class BannerLine;
class CmBrush;

namespace  Gdiplus
{
class Graphics;
class RectF;
class GraphicsPath;
}

enum  class AlignMode
{
	Center,
	BottomRight,
	TopLeft
};

enum class ShadowType { Rear, Fore, Baseline };
enum class BlockDirection { TopLeft, TopRight, BottomLeft, BottomRight };
enum class PerspectiveDirection { Down, Up };
//----------------------------------------------------------------------------
class TextFx
{
public:
	TextFx()
	{
		AddColorPropDefault();
	}

	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr,
		                  const Gdiplus::RectF& rect) = 0;
	virtual ~TextFx() {}

	void			SetColorPropertyValue(ColorPropertyClass id, std::wstring colorName);
	std::wstring	GetColorPropertyValue(ColorPropertyClass id);
	int				GetColorPropertyCount() const { return static_cast<int>(m_colorPropList.size()); }
	ColorProperty	GetColorPropertyItem(int index) const { return m_colorPropList[index]; }

protected:
	void			AddColorPropDefault();
	void			DrawLineBackground(Gdiplus::Graphics & gr, const Gdiplus::RectF & lineRect);

	void			AlignScalePath(std::vector<Gdiplus::GraphicsPath*> pathList, 
					const Gdiplus::RectF& lineRect,
					AlignMode = AlignMode::Center);

	ColorPropertyList				m_colorPropList;
};

//----------------------------------------------------------------------------
//
// Available text renderers declarations 
//
//----------------------------------------------------------------------------
class TextFxSolid : public TextFx
{
public:
	TextFxSolid() : TextFx() 
	{
		m_colorPropList.emplace_back(ColorPropertyClass::Face_Outline, L"Black");
	};
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxBlock : public TextFx
{
public:
	explicit TextFxBlock(BlockDirection blockDir) : m_blockDir(blockDir),TextFx() 
	{
		m_colorPropList.emplace_back(ColorPropertyClass::Face_Outline, L"Black");
		m_colorPropList.emplace_back(ColorPropertyClass::Shade_1, L"Orange 1");
		m_colorPropList.emplace_back(ColorPropertyClass::Shade_2, L"Orange 2");
		m_colorPropList.emplace_back(ColorPropertyClass::Shade_3, L"Orange 3");
		m_colorPropList.emplace_back(ColorPropertyClass::Shade_4, L"Orange 4");
		m_colorPropList.emplace_back(ColorPropertyClass::Shade_5, L"Orange 5 Pure");
		m_colorPropList.emplace_back(ColorPropertyClass::Shade_6, L"Orange 6");	
	}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
private:
	BlockDirection m_blockDir;
};
//----------------------------------------------------------------------------

class TextFxShadow : public TextFx
{
public:
	explicit TextFxShadow(ShadowType shType) : m_shadowType(shType), TextFx()
	{
		m_colorPropList.emplace_back(ColorPropertyClass::Shadow, L"Black");
		m_colorPropList.emplace_back(ColorPropertyClass::Shadow_Outline, L"Black");
		m_colorPropList.emplace_back(ColorPropertyClass::Face_Outline, L"Black");
	}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;

private:
	ShadowType m_shadowType;
};
//----------------------------------------------------------------------------

class TextFxPerspective : public TextFx
{
public:
	explicit TextFxPerspective(PerspectiveDirection dir) : m_direction(dir), TextFx()
	{
		m_colorPropList.emplace_back(ColorPropertyClass::Face_Outline, L"Black");
		m_colorPropList.emplace_back(ColorPropertyClass::Sides, L"Black");
	}

	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
private:
	PerspectiveDirection m_direction;
};

//----------------------------------------------------------------------------

class TextFxBlend : public TextFx
{
public:
	TextFxBlend() : TextFx() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxTwoOutlines : public TextFx
{
public:
	TextFxTwoOutlines() : TextFx() 
	{
		m_colorPropList.emplace_back(ColorPropertyClass::Outer_Outline, L"Aqua");
		m_colorPropList.emplace_back(ColorPropertyClass::Inner_Outline, L"Yellow 2");
		SetColorPropertyValue(ColorPropertyClass::Face, L"Orange 2");
	}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
	void SetOutline2Width(float f) { m_outlineWidth2 = f; }
private:
	float		m_outlineWidth2 = 1.0f;
};

//----------------------------------------------------------------------------