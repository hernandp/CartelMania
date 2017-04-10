#pragma once
#include <string>
#include <memory>

class TextFXRenderer;
namespace Gdiplus
{
class Graphics;
class RectF;
}

// ----------------------------------------------------------------------------

class BannerLine
{
public:
	BannerLine(const std::wstring& text,
		const std::wstring& fontName,
		std::unique_ptr<TextFXRenderer> effect);

	void SetTextFx(std::unique_ptr<TextFXRenderer> newFx);
	void DrawOn(Gdiplus::Graphics& gr, const Gdiplus::RectF& rect) const;
	const std::wstring& GetText() const { return m_text; }
	const std::wstring& GetFontName() const { return m_fontName; }

private:
	std::unique_ptr<TextFXRenderer> m_textFx;
	std::wstring					m_text;
	std::wstring					m_fontName;
};

// ----------------------------------------------------------------------------
