#ifndef _BANNER_H_
#define _BANNER_H_

#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <gdiplus.h>
#include "textrend.h"

// ----------------------------------------------------------------------------
//
// Constants for banners and defaults
//
// ----------------------------------------------------------------------------

const float BANNER_MARGIN_PX = 20;
const float BANNER_HEIGHT_PCT = 50;
const auto  DEFAULT_LINE1_TEXT{ L"Line 1" };
const auto  DEFAULT_LINE2_TEXT{ L"Line 2" };
const auto  DEFAULT_FONT_NAME{ L"Arial" };

// ----------------------------------------------------------------------------

enum class BannerLayout
{
	SingleLine,
	SmallOverLarge3,
	SmallOverLarge2,
	SmallOverLarge1,
	MediumMedium,
	LargeOverSmall1,
	LargeOverSmall2,
	LargeOverSmall3
};

// ----------------------------------------------------------------------------

class BannerLine
{	
public:
	BannerLine(const std::wstring& text,
		const std::wstring& fontName,
		std::unique_ptr<TextFXRenderer> effect) :
		m_textFx(std::move(effect)),
		m_text(text),
		m_fontName(fontName)
	{
	}

	void SetTextFx(std::unique_ptr<TextFXRenderer> newFx)
	{
		m_textFx = move(newFx);
	}

	void DrawOn(Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) const
	{
		m_textFx->DrawLine(*this, gr, rect);
	}

	const std::wstring& GetText() const { return m_text;  }
	const std::wstring& GetFontName() const { return m_fontName; }

private:
	std::unique_ptr<TextFXRenderer> m_textFx;
	std::wstring					m_text;
	std::wstring					m_fontName;
};

// ----------------------------------------------------------------------------

class Banner
{
public:
	Banner();
	~Banner();
	
	BannerLine& GetLine(int index) { return m_lines.at(index);  }
	BannerLayout GetLayout() const { return m_layout;  }
	void SetLayout(BannerLayout layout) { m_layout = layout;  }
	void PaintOn(HDC hdc, const LPRECT rcClient);

private:
	BannerLayout				m_layout;
	std::vector<BannerLine>		m_lines;
};


#endif //_BANNER_H_