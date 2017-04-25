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
		AddColorPropEx();
	}

	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr,
		const Gdiplus::RectF& rect) = 0;
	virtual ~TextFXRenderer() {}

	void SetOutlineWidth(float w) { m_outlineWidth = w; }
	void SetColorProperty(ColorPropertyClass id, std::wstring colorName);

	size_t GetColorPropertyCount() const { return m_colorPropList.size(); }
	ColorProperty GetColorPropertyItem(int index) const { return m_colorPropList[index]; }

protected:
	void AddColorPropDefault();
	virtual void AddColorPropEx() 
	{

	}
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
	TextFxSolid() : TextFXRenderer() {};
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxBlock : public TextFXRenderer
{
	//virtual void AddColorPropEx() override;
public:
	TextFxBlock() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxShadowRear : public TextFXRenderer
{
	//virtual void AddColorPropEx() override;
public:
	TextFxShadowRear() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxBlend : public TextFXRenderer
{
	//virtual void AddColorPropEx() override;
public:
	TextFxBlend() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxTwoOutlines : public TextFXRenderer
{
	//virtual void AddColorPropEx() override;
public:
	TextFxTwoOutlines() : TextFXRenderer() {}
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
	void SetOutline2Width(float f) { m_outlineWidth2 = f; }
private:
	float		m_outlineWidth2 = 1.0f;
};

//----------------------------------------------------------------------------