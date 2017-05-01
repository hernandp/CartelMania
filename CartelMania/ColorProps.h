#pragma once
#include <vector>
#include <unordered_map>

enum class ColorPropertyClass
{
	Background,
	Background_Outline,
	Face,
	Face_Outline,
	Outer_Outline,
	Inner_Outline,
	Outermost_Outline,
	Outline_2,
	Outline_3,
	Outline_4,
	Innermost_Outline,
	Front_Face,
	Front_Face_Outline,
	Middle_Face,
	Middle_Face_Outline,
	Back_Face,
	Back_Face_Outline,
	Sides,
	Side_Outline,
	Shade_1,
	Shade_2,
	Shade_3,
	Shade_4,
	Shade_5,
	Shade_6,
	Wireframe,
	Shadow,
	Shadow_Outline,
	Accordion,
	Accordion_Outline,
	Balloon_1,
	Balloon_2,
	Balloon_3,
	Balloon_Outline,
	String,
	Highlight,
	Highlight_Outline
};
// ----------------------------------------------------------------------------

inline const wchar_t* ColorPropertyClassToW(ColorPropertyClass pc)
{
	static std::unordered_map<ColorPropertyClass, const wchar_t*> strMap{
	{ ColorPropertyClass::Background, L"Background" },
	{ ColorPropertyClass::Background_Outline, L"Background Outline" },
	{ ColorPropertyClass::Face, L"Face"},
	{ ColorPropertyClass::Face_Outline, L"Face Outline"},
	{ ColorPropertyClass::Outer_Outline, L"Outer Outline"},
	{ ColorPropertyClass::Inner_Outline, L"Inner Outline"},
	{ ColorPropertyClass::Outermost_Outline, L"Outermost Outline"},
	{ ColorPropertyClass::Outline_2, L"Outline 2"},
	{ ColorPropertyClass::Outline_3, L"Outline 3"},
	{ ColorPropertyClass::Outline_4, L"Outline 4"},
	{ ColorPropertyClass::Innermost_Outline, L"Innermost Outline"},
	{ ColorPropertyClass::Front_Face, L"Front Face"},
	{ ColorPropertyClass::Front_Face_Outline, L"Front Face Outline"},
	{ ColorPropertyClass::Middle_Face, L"Middle Face"},
	{ ColorPropertyClass::Middle_Face_Outline, L"Middle Face Outline"},
	{ ColorPropertyClass::Back_Face, L"Back Face"},
	{ ColorPropertyClass::Back_Face_Outline, L"Back Face Outline"},
	{ ColorPropertyClass::Sides, L"Sides"},
	{ ColorPropertyClass::Side_Outline, L"Side Outline"},
	{ ColorPropertyClass::Shade_1, L"Shade 1"},
	{ ColorPropertyClass::Shade_2, L"Shade 2"},
	{ ColorPropertyClass::Shade_3, L"Shade 3"},
	{ ColorPropertyClass::Shade_4, L"Shade 4"},
	{ ColorPropertyClass::Shade_5, L"Shade 5"},
	{ ColorPropertyClass::Shade_6, L"Shade 6"},
	{ ColorPropertyClass::Wireframe, L"Wireframe"},
	{ ColorPropertyClass::Shadow, L"Shadow"},
	{ ColorPropertyClass::Shadow_Outline, L"Shadow Outline"},
	{ ColorPropertyClass::Accordion, L"Accordion"},
	{ ColorPropertyClass::Accordion_Outline, L"Accordion Outline"},
	{ ColorPropertyClass::Balloon_1, L"Balloon 1"},
	{ ColorPropertyClass::Balloon_2, L"Balloon 2"},
	{ ColorPropertyClass::Balloon_3, L"Balloon 3"},
	{ ColorPropertyClass::Balloon_Outline, L"Balloon 4"},
	{ ColorPropertyClass::String, L"String"},
	{ ColorPropertyClass::Highlight, L"Highlight"},
	{ ColorPropertyClass::Highlight_Outline, L"Highlight Outline"}
	};

	return strMap[pc];
};

// ----------------------------------------------------------------------------

class ColorProperty
{
public:
	ColorProperty(ColorPropertyClass _class, const std::wstring& value)
		: m_class(_class), m_value(value)
	{
	}

	ColorPropertyClass GetClass() const { return m_class; }
	const wchar_t* GetClassString() const { return ColorPropertyClassToW(m_class); }
	void SetClass(ColorPropertyClass val) { m_class = val; }

	std::wstring GetValue() const { return m_value; }
	const wchar_t* GetValueW() const { return m_value.c_str();  }
	void SetValue(const std::wstring& val) { m_value = val; }

private:
	ColorPropertyClass  m_class;
	std::wstring		m_value;
};

using ColorPropertyList = std::vector<ColorProperty>;

// ----------------------------------------------------------------------------