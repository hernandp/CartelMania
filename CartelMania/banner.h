#pragma once
#include <map>
#include <windows.h>
#include <memory>

class BannerLine;

// ----------------------------------------------------------------------------
//
// Constants for banners and defaults
//
// ----------------------------------------------------------------------------

const float BANNER_MARGIN_PX = 20;
const float BANNER_HEIGHT_PCT = 50;
const auto  DEFAULT_TOPLINE_TEXT{ L"CartelMania" };
const auto  DEFAULT_BOTTOMLINE_TEXT{ L"Line 2" };
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

// Returns true if both layouts contain the same number of lines
//
inline bool AreCompatibleLayouts(BannerLayout l1, BannerLayout l2)
{
	if ((l1 == l2) ||
		(l1 != BannerLayout::SingleLine && l2 != BannerLayout::SingleLine))
	{
		return true;
	}

	return false;
}

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
class CManiaMainWnd;
class Banner
{
public:
	Banner();
	~Banner();
	
	BannerLine* GetTopLine() const {	return m_topLine.get();	}
	BannerLine* GetBottomLine() const { return m_bottomLine.get(); }
	BannerLayout GetLayout() const { return m_layout;  }
	const std::wstring& GetShapeName() const { return m_shapeName;  }
	void SetShapeName(const std::wstring& name) { m_shapeName = name;  }
	void SetLayout(BannerLayout layout) { m_layout = layout;  }
	void PaintOn(HDC hdc, const LPRECT rcClient);
	void Invalidate();

private:
	void DrawSelectionMark(Gdiplus::Graphics & gr, const Gdiplus::RectF& bannerRect);
	void BuildPaths();

	BannerLayout								m_layout;
	std::wstring								m_shapeName;
	std::unique_ptr<BannerLine>					m_topLine;
	std::unique_ptr<BannerLine>					m_bottomLine;
};
