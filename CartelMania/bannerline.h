#pragma once
#include <string>
#include <memory>

// ----------------------------------------------------------------------------

class TextFx;

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
	BannerLine(
		const std::wstring& defaultText,
		const std::wstring& text,
		const std::wstring& fontName,
		Gdiplus::FontStyle fontStyle,
		std::unique_ptr<TextFx> effect);

	void				SetTextFx(std::unique_ptr<TextFx> newFx);
	TextFx*				GetTextFx() const { return m_textFx.get();  }
	void				DrawOn(Gdiplus::Graphics& gr, const Gdiplus::RectF& rect);
	const std::wstring& GetText() const { return m_text; }
	void				SetText(const std::wstring& text);
	std::wstring		GetDefaultText() const { return m_defaultText;  }
	const std::wstring& GetFontName() const { return m_fontName; }

	Gdiplus::GraphicsPath* GetPath(); 
	std::unique_ptr<Gdiplus::GraphicsPath> GetPathCopy();

	void InvalidatePath() { m_needRegen = true; }

private:
	void BuildPath();

	std::unique_ptr<TextFx>					m_textFx;
	std::wstring							m_text, m_defaultText;
	std::wstring							m_fontName;
	Gdiplus::FontStyle						m_fontStyle;
	std::unique_ptr<Gdiplus::GraphicsPath>	m_path;
	bool									m_needRegen;
};

// ----------------------------------------------------------------------------
