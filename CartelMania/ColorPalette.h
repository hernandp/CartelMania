#pragma once
#include "ColorProps.h"
#include <vector>

class ColorPalette
{
	explicit ColorPalette(std::wstring name) : m_name(name) {}

private:
	std::wstring				m_name;
	std::vector<ColorProperty>	m_palette;
};