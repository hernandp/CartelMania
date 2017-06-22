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

enum class ShadowType { Rear, Fore, Baseline  };

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
	TextFxBlock() : TextFx() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
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