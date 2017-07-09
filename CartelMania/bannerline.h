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
		const std::wstring& effect);

	void				SetTextFx(const std::wstring& fxName);
	TextFx*				GetTextFx() const { return m_textFx.get(); }
	const std::wstring& GetTextFxName() const { return m_textFxName;  }
	void				DrawOn(Gdiplus::Graphics& gr, const Gdiplus::RectF& rect);
	const std::wstring& GetText() const { return m_text; }
	void				SetText(const std::wstring& text);
	std::wstring		GetDefaultText() const { return m_defaultText;  }
	const std::wstring& GetFontName() const { return m_fontName; }
	void				SetFontName(const std::wstring& fontName) { m_fontName = fontName;  }
	bool				IsFontStyleBold() const { return m_fontStyle == Gdiplus::FontStyleBold; }
	void				SetFontStyleBold(bool f) { m_fontStyle = f ?  Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular;  }
	void				SetShapeName(const std::wstring& name) { m_shapeName = name;  }
	const std::wstring&	GetShapeName() const {	return m_shapeName;	}
	Gdiplus::GraphicsPath* GetPath(); 
	std::unique_ptr<Gdiplus::GraphicsPath> GetPathCopy();

	void InvalidatePath() { m_needRegen = true; }

private:
	void BuildPath();

	std::unique_ptr<TextFx>					m_textFx;
	std::wstring							m_textFxName;
	std::wstring							m_text;
	std::wstring							m_defaultText;
	std::wstring							m_fontName;
	std::wstring							m_shapeName;
	Gdiplus::FontStyle						m_fontStyle;
	std::unique_ptr<Gdiplus::GraphicsPath>	m_path;
	bool									m_needRegen;
};

// ----------------------------------------------------------------------------
