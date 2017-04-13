#ifndef _BANNER_H_
#define _BANNER_H_

#include "textrend.h"
#include "bannerline.h"
#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <gdiplus.h>
#include <map>

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

// 
// Approximate "empirical" calculation from Bannermania
// Prop1: 25% / 75 %
// Prop2: 33.3% / 66.6 %
// Prop3: 40% / 60 %

const std::map<BannerLayout, std::pair<float, float>> g_proportionTable =
{
	{BannerLayout::LargeOverSmall1, std::make_pair(0.6f, 0.4f)},
	{BannerLayout::LargeOverSmall2, std::make_pair(0.666666f, 0.3333333f)},
	{BannerLayout::LargeOverSmall3, std::make_pair(0.75f, 0.25f)},
	{BannerLayout::MediumMedium,    std::make_pair(0.5f, 0.5f)},
	{BannerLayout::SingleLine,      std::make_pair(1.0f, 0.0f)},
	{BannerLayout::SmallOverLarge1, std::make_pair(0.4f, 0.6f)},
	{BannerLayout::SmallOverLarge2, std::make_pair(0.333333f, 0.666666f)},
	{BannerLayout::SmallOverLarge3, std::make_pair(0.25f, 0.75f)}
};

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