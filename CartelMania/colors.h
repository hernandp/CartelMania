#pragma once
#include <gdiplus.h>


// *** all of our symbols must be into namespaces ***

namespace Cartelmania
{
enum class FillType
{
	Solid,
	Hatch,
	Texture,
	Gradient
};

struct Color
{
	const wchar_t*		m_name;
	FillType			m_type;
	Gdiplus::HatchStyle m_hatch;	// valid only if m_type == Hatch
	Gdiplus::Color		m_fg;
	Gdiplus::Color		m_bg;
};

extern const Color g_Colors[];

}