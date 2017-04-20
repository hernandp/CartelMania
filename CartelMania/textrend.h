#pragma once

//----------------------------------------------------------------------------
//
// Forward declarations
//
//----------------------------------------------------------------------------
class Banner;
class BannerLine;
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
	//
	// Renders text on target graphics object.
	//
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, 
		const Gdiplus::RectF& rect) = 0;
	
	//
	// Common members to all text renderers 
	//
	void SetOutlineWidth(float w) { m_outlineWidth = w; }

protected:
	void AlignScalePath(Gdiplus::GraphicsPath* path, const Gdiplus::RectF& lineRect);

	float				m_outlineWidth = 1.0f;
};

//----------------------------------------------------------------------------
//
// Available text renderers declarations 
//
//----------------------------------------------------------------------------
class TextFxSolid : public TextFXRenderer
{
public:
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxBlock : public TextFXRenderer
{
public:
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};
//----------------------------------------------------------------------------

class TextFxShadowRear : public TextFXRenderer
{
public:
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxBlend : public TextFXRenderer
{
public:
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
};

//----------------------------------------------------------------------------

class TextFxTwoOutlines : public TextFXRenderer
{
public:
	virtual void DrawLine(BannerLine& line, Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) override;
	void SetOutline2Width(float f) { m_outlineWidth2 = f; }
private:
	float		m_outlineWidth2 = 1.0f;
};

//----------------------------------------------------------------------------