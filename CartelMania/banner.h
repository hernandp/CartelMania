#ifndef _BANNER_H_
#define _BANNER_H_

#include <windows.h>
#include <string>
#include <memory>

class TextFXRenderer;
using ITextFXRendererPtr = std::unique_ptr<TextFXRenderer>;

// ----------------------------------------------------------------------------
//
// Constants for banner display, size and placement on display area
//
// ----------------------------------------------------------------------------

const float BANNER_MARGIN_PX = 20;
const float BANNER_HEIGHT_PCT = 50;

// ----------------------------------------------------------------------------

class Banner
{
public:
	Banner();
	~Banner();
	
	void SetText1(const std::wstring& text) { m_text[0] = text; }
	void SetText2(const std::wstring& text) { m_text[1] = text; }
	std::wstring GetText1() const { return m_text[0]; }
	std::wstring GetText2() const { return m_text[1]; }
	void SetFont(const std::wstring& fontName) { m_fontName = fontName; }
	void SetTextRenderer(ITextFXRendererPtr pTextRenderer);
	const std::wstring GetFont() const { return m_fontName; }
	void PaintOn(HDC hdc, const LPRECT rcClient);

private:
	std::wstring		m_text[2];
	std::wstring		m_fontName;
	ITextFXRendererPtr	m_textRenderer;
	//Layout*		m_layout;
	//Palette*		m_palette;
};


#endif //_BANNER_H_