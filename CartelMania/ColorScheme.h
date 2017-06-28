#pragma once
#include "ColorProps.h"
#include <vector>

class ColorScheme
{
public:
	explicit ColorScheme(const std::wstring& name) : m_name(name) {}

private:
	std::wstring				m_name;
	std::vector<ColorProperty>	m_palette;
};