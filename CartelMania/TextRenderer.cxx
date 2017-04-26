#include "stdafx.h"
#include "TextRenderer.h"
#include "bannerline.h"
#include "GlobalSettings.h"
#include "Geometry.h"
#include "colors.h"

using namespace Gdiplus;
using namespace std;

extern GlobalSettings g_globalSettings;

//-----------------------------------------------------------------------------

void TextFXRenderer::SetColorPropertyValue(ColorPropertyClass id, std::wstring colorName)
{
	auto it = std::find_if(m_colorPropList.begin(), m_colorPropList.end(),
		[id](ColorProperty& cp)
	{
		return (cp.GetClass() == id);
	}
	);

	if (it != m_colorPropList.end())
	{
		it->SetValue(colorName);
	}
}

//-----------------------------------------------------------------------------

std::wstring TextFXRenderer::GetColorPropertyValue(ColorPropertyClass id)
{
	auto it = std::find_if(m_colorPropList.begin(), m_colorPropList.end(),
		[id](ColorProperty& cp)
	{
		return (cp.GetClass() == id);
	}
	);

	if (it != m_colorPropList.end())
	{
		return it->GetValue();
	}
	else
	{
		throw (invalid_argument("invalid color property class ID"));
	}
}

//-----------------------------------------------------------------------------

void TextFXRenderer::AddColorPropDefault()
{
	m_colorPropList.emplace_back(ColorPropertyClass::Background, L"White");
	m_colorPropList.emplace_back(ColorPropertyClass::Background_Outline, L"Black");
	m_colorPropList.emplace_back(ColorPropertyClass::Face, L"Violet");
}

void TextFXRenderer::DrawLineBackground(Graphics& gr, const RectF& lineRect)
{
	auto bgColor = GetColorPropertyValue(ColorPropertyClass::Background);
	auto outColor = GetColorPropertyValue(ColorPropertyClass::Background_Outline);
	gr.FillRectangle(GetBrushFromColorTable(bgColor), lineRect);
	gr.DrawRectangle(&Pen(GetBrushFromColorTable(outColor), 1.0f), lineRect);
}

//-----------------------------------------------------------------------------

void TextFXRenderer::AlignScalePath(GraphicsPath* path, const RectF& lineRect)
{
	RectF bounds;
	path->GetBounds(&bounds);

	Matrix vAlign;
	vAlign.Translate((lineRect.Width / 2 - bounds.Width / 2) - bounds.X, (lineRect.Height / 2 - bounds.Height / 2) - bounds.Y);
	path->Transform(&vAlign);

	// Calculate how much X/Y-scaling we need.
	// Mantain aspect-ratio (sX=sY)

	const REAL ratio = bounds.Width / bounds.Height;
	const REAL s = lineRect.Height / bounds.Height;

	Matrix mtx;
	mtx.Translate(lineRect.Width / 2.0f, lineRect.Height / 2.0f);
	mtx.Scale(s, s);
	mtx.Translate(-lineRect.Width / 2.0f, -lineRect.Height / 2.0f);
	path->Transform(&mtx);
}

/************************************************************************/
//
// Effect implementations start here                                    
//
/************************************************************************/

//-----------------------------------------------------------------------------
//
// Solid Effect
//
//-----------------------------------------------------------------------------

void TextFxSolid::DrawLine(BannerLine& line, _In_ Graphics& gr, _In_ const RectF& lineRect)
{
	GraphicsPath* path = line.GetPath();
	//auto path = unique_ptr<GraphicsPath>(WarpPath(*origPath));
	AlignScalePath(path, lineRect);
	DrawLineBackground(gr, lineRect);

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto faceOutline = GetColorPropertyValue(ColorPropertyClass::Face_Outline);
	
	if (!g_globalSettings.m_fDebugDisableFillPath)
		gr.FillPath(GetBrushFromColorTable(faceColor), path);

	if (g_globalSettings.m_fDebugDrawVertices)
		DrawPathVertices(gr, *path);

	gr.DrawPath(&Pen(GetBrushFromColorTable(faceOutline), 1), path);
}

//-----------------------------------------------------------------------------
//
// Two Outline effect
//
//-----------------------------------------------------------------------------

void TextFxTwoOutlines::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	GraphicsPath* path = line.GetPath();
	//auto path = unique_ptr<GraphicsPath>(WarpPath(*origPath));

	AlignScalePath(path, lineRect);
	DrawLineBackground(gr, lineRect);

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto innerOutline = GetColorPropertyValue(ColorPropertyClass::Inner_Outline);
	auto outerOutline = GetColorPropertyValue(ColorPropertyClass::Outer_Outline);
	
	Pen penIn(GetBrushFromColorTable(innerOutline), m_outlineWidth);
	Pen penOut(GetBrushFromColorTable(outerOutline), m_outlineWidth * 2);
	const Brush* brush = GetBrushFromColorTable(faceColor);
	
	gr.DrawPath(&penOut, path);
	gr.DrawPath(&penIn, path);
	gr.FillPath(brush, path);
}

//-----------------------------------------------------------------------------

void TextFxShadowRear::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	//StringFormat format;
	//format.SetTrimming(StringTrimmingNone);
	//format.SetFormatFlags(StringFormatFlagsNoWrap);

	//RectF boundingBox;
	//FontFamily family(line.GetFontName().c_str());
	//unique_ptr<Font> font = FindFontToFillTextInRect(gr, lineRect, family, line.GetText(), format, &boundingBox);

	//RectF rcBounds = QueryStringPathBounds(gr, line.GetText(), *font, lineRect, format);
	//	
	//// Shear matrix by K units yields for (x,y):
	//// x' = x + Ky
	//// y' = y + Kx
	////
	//// K is the tangent of the shearing angle (so K=1 -> 45 deg)
	//
	//
	//Matrix mtx;
	//mtx.Shear(-0.2f, 0.0f);
	////mtx.Scale(1.0f, -1.0f);
	////mtx.Translate(-0.2f, 0.0f);	

	//rcBounds.X = rcBounds.Y = 0;

	/*GraphicsPath p;
	p.AddRectangle(rcBounds);
	p.Transform(&mtx);
	gr.FillPath(&SolidBrush(Gdiplus::Color::Red), &p);
	gr.DrawRectangle(&Pen(Gdiplus::Color::Black), rcBounds);
	
	DrawStringPath(gr, g_Colors[0], banner.GetText1(), rcBounds, format, *font.get(), &mtx);
	DrawStringPath(gr, g_Colors[2], banner.GetText1(), boundingBox, format, *font.get());*/
}

//-----------------------------------------------------------------------------

void TextFxBlock::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	//StringFormat format;
	//format.SetAlignment(StringAlignmentCenter);
	//format.SetLineAlignment(StringAlignmentCenter);
	//format.SetTrimming(StringTrimmingNone);
	//format.SetFormatFlags(StringFormatFlagsNoWrap);

	//FontFamily family(line.GetFontName().c_str());
	//unique_ptr<Font> fillFont = FindFontToFillTextInRect(gr, lineRect, family, line.GetText(), format);

	//// Cheap block effect

	//const int BLOCKDEPTH = 128;

	//for (int i = 0; i < BLOCKDEPTH; ++i)
	//{
	//	RectF rcBlock = lineRect;
	//	const BYTE layerColorComp = BYTE(255.0f * ((BLOCKDEPTH - i) / REAL(BLOCKDEPTH)));
	//	rcBlock.Offset(REAL(BLOCKDEPTH - i), REAL(BLOCKDEPTH - i));

	//	gr.DrawString(line.GetText().c_str(), -1, fillFont.get(), rcBlock, &format,
	//		&SolidBrush(Gdiplus::Color(layerColorComp, layerColorComp, layerColorComp)));
	//}

	//// Draw at the choosen font size
	////

	//gr.DrawString(line.GetText().c_str(), -1, fillFont.get(), lineRect, &format,
	//	&HatchBrush(HatchStyle20Percent, Gdiplus::Color::Blue, Gdiplus::Color::Red));
}

//-----------------------------------------------------------------------------

void TextFxBlend::DrawLine(BannerLine& line, Graphics& gr, const RectF& bannerRect)
{
	//StringFormat format;
	//format.SetTrimming(StringTrimmingNone);
	//format.SetFormatFlags(StringFormatFlagsNoWrap);
	//format.SetAlignment(StringAlignmentCenter);

	//RectF boundingBox;
	//FontFamily family(line.GetFontName().c_str());
	//unique_ptr<Font> font = FindFontToFillTextInRect(gr, bannerRect, family, line.GetText(), format, &boundingBox);

	//// Center vertically [ user could choose other alignments ]
	////
	//Matrix mtx;
	//mtx.Translate(0.0f, (bannerRect.Height / 2) - (boundingBox.Height / 2));

	//auto fontSize = PointToPixels(gr, font->GetSize());
	//auto fontStyle = font->GetStyle();

	//GraphicsPath path;
	//Pen penIn(Gdiplus::Color::Yellow, m_outlineWidth);
	//Pen penOut(Gdiplus::Color::Blue, m_outlineWidth * 2);
	//HatchBrush brush(g_Colors[2].m_hatch, g_Colors[2].m_fg, g_Colors[2].m_bg);
	//path.AddString(line.GetText().c_str(), -1, &family, fontStyle, fontSize, boundingBox, &format);
	//path.Transform(&mtx);

	//gr.DrawPath(&penOut, &path);
	//gr.DrawPath(&penIn, &path);
	//gr.FillPath(&brush, &path);
}