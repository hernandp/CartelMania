#pragma once
#include <comutil.h>
#include <map>

//----------------------------------------------------------------------------
//
// Handy macros for nicer code
//
//----------------------------------------------------------------------------
#define DECLARE_TEXTFX(_class_name_) \
	class _class_name_ : public TextFXRenderer \
	{											\
	public: void Draw(const Banner& banner, Gdiplus::Graphics& gr, const Gdiplus::RectF& bannerRect); \
    };											\

//----------------------------------------------------------------------------
//
// Available properties
//
//----------------------------------------------------------------------------
enum class TextProp
{
	//OutlineWidth
};

//----------------------------------------------------------------------------
//
// Forward declarations
//
//----------------------------------------------------------------------------
class Banner;
namespace  Gdiplus
{
class Graphics;
class RectF;
}

//----------------------------------------------------------------------------

class TextFXRenderer
{
public:
	//
	// Renders text on target graphics object.
	//
	virtual void Draw(const Banner& banner, Gdiplus::Graphics& gr, const Gdiplus::RectF& bannerRect) = 0;
	
	//
	// Common members to all text renderers 
	//
	void SetOutlineWidth(float w) { m_outlineWidth = w; }

protected:
	float		m_outlineWidth = 1.0f;
};

//----------------------------------------------------------------------------
//
// All renderers without additional private members are declared here. 
// See proper .CXX source for definition.
// (start with textfx_ )
//
//----------------------------------------------------------------------------

DECLARE_TEXTFX(TextFxSolid);
DECLARE_TEXTFX(TextFxBlock);
DECLARE_TEXTFX(TextFxShadowRear);

//----------------------------------------------------------------------------

class TextFxTwoOutlines : public TextFXRenderer
{
public:
	virtual void Draw(const Banner& banner, Gdiplus::Graphics& gr, const Gdiplus::RectF& bannerRect);
	void SetOutline2Width(float f) { m_outlineWidth2 = f; }
private:
	float		m_outlineWidth2 = 1.0f;
};