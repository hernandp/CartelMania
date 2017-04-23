#pragma once
#include <string>
#include <memory>

// ----------------------------------------------------------------------------

class TextFXRenderer;

namespace Gdiplus
{
enum FontStyle;
class GraphicsPath;
class Graphics;
class RectF;
}

// ----------------------------------------------------------------------------

class BannerLine
{
public:
	BannerLine(const std::wstring& text,
		const std::wstring& fontName,
		Gdiplus::FontStyle fontStyle,
		std::unique_ptr<TextFXRenderer> effect);

	void SetTextFx(std::unique_ptr<TextFXRenderer> newFx);
	void DrawOn(Gdiplus::Graphics& gr, const Gdiplus::RectF& rect);
	const std::wstring& GetText() const { return m_text; }
	const std::wstring& GetFontName() const { return m_fontName; }
	Gdiplus::GraphicsPath* GetPath(); 
	void InvalidatePath() { m_needRegen = true; }

private:
	void BuildPath();

	std::unique_ptr<TextFXRenderer>			m_textFx;
	std::wstring							m_text;
	std::wstring							m_fontName;
	Gdiplus::FontStyle						m_fontStyle;
	std::unique_ptr<Gdiplus::GraphicsPath>	m_path;
	bool									m_needRegen;
};

// ----------------------------------------------------------------------------
