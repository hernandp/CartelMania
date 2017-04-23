#pragma once
#include <gdiplus.h>
#include <memory>

inline Gdiplus::Color MakeRandColor()
{
	return Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
}

class CmBrush
{
public:
	CmBrush(const std::wstring& name, 
		std::unique_ptr<Gdiplus::Brush> brush) : m_name(name), m_brush(std::move(brush))
	{ }

	//CmBrush(CmBrush&& b) : m_name(b.m_name), m_brush(std::move(b.m_brush)) {}

	std::wstring GetName() const { return m_name;  }
	const Gdiplus::Brush* GetBrush() const { return m_brush.get(); }

private:
	std::wstring						m_name;
	std::unique_ptr<Gdiplus::Brush>		m_brush;
};

// The classic Bannermania Colors

extern const CmBrush g_bmColors[];
extern const int g_bmColorsCount;


