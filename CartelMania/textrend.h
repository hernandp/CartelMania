#pragma once

//----------------------------------------------------------------------------
//
// Handy macro for nicer code
//
//----------------------------------------------------------------------------
#define DECLARE_TEXTFX(_class_name_) \
	class _class_name_ : public ITextFXRenderer \
	{											\
	public: void Draw(const Banner& banner, Gdiplus::Graphics& gr, Gdiplus::RectF& rc); \
    };											\

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

class ITextFXRenderer
{
public:
	//
	// Renders text on target graphics object.
	//
	
	virtual void Draw(const Banner& banner, Gdiplus::Graphics& gr, Gdiplus::RectF& rc) = 0;
};

//----------------------------------------------------------------------------
//
// All renderers are declared here. See proper .CXX source for definition.
// (start with textfx_ )
//
//----------------------------------------------------------------------------

DECLARE_TEXTFX(TextFxSolid);
DECLARE_TEXTFX(TextFxBlock);
