#pragma once
#include "ColorProps.h"
#include "Colors.h"

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

//----------------------------------------------------------------------------
class TextFXRenderer
{
public:
	TextFXRenderer()
	{
		AddColorPropDefault();
	}

	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr,
		const Gdiplus::RectF& rect) = 0;
	virtual ~TextFXRenderer() {}

	void SetOutlineWidth(float w) { m_outlineWidth = w; }

	void SetColorPropertyValue(ColorPropertyClass id, std::wstring colorName);
	std::wstring GetColorPropertyValue(ColorPropertyClass id);

	size_t GetColorPropertyCount() const { return m_colorPropList.size(); }
	ColorProperty GetColorPropertyItem(int index) const { return m_colorPropList[index]; }

protected:
	void AddColorPropDefault();
	void DrawLineBackground(Gdiplus::Graphics & gr, const Gdiplus::RectF & lineRect);

	void AlignScalePath(Gdiplus::GraphicsPath* path, const Gdiplus::RectF& lineRect);

	float							m_outlineWidth = 1.0f;
	ColorPropertyList				m_colorPropList;
};

//----------------------------------------------------------------------------
//
// Available text renderers declarations 
//
//----------------------------------------------------------------------------
class TextFxSolid : public TextFXRenderer
{
public:
	TextFxSolid() : TextFXRenderer() 
	{
		m_colorPropList.emplace_back(ColorPropertyClass::Face_Outline, L"Black");
	};
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxBlock : public TextFXRenderer
{
public:
	TextFxBlock() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxShadowRear : public TextFXRenderer
{
public:
	TextFxShadowRear() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxBlend : public TextFXRenderer
{
public:
	TextFxBlend() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxTwoOutlines : public TextFXRenderer
{
public:
	TextFxTwoOutlines() : TextFXRenderer() 
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