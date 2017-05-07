#include "stdafx.h"
#include "TextFx.h"
#include "bannerline.h"
#include "AppSettings.h"
#include "Geometry.h"
#include "colorTable.h"
#include "CartelManiaApp.h"
#include "debug.h"
#include "vld.h"

using namespace Gdiplus;
using namespace std;

extern AppSettings g_globalSettings;

//-----------------------------------------------------------------------------

void TextFx::SetColorPropertyValue(ColorPropertyClass id, std::wstring colorName)
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

std::wstring TextFx::GetColorPropertyValue(ColorPropertyClass id)
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

void TextFx::AddColorPropDefault()
{
	m_colorPropList.emplace_back(ColorPropertyClass::Background, L"White");
	m_colorPropList.emplace_back(ColorPropertyClass::Background_Outline, L"Black");
	m_colorPropList.emplace_back(ColorPropertyClass::Face, L"Violet");
}

void TextFx::DrawLineBackground(Graphics& gr, const RectF& lineRect)
{
	auto bgColor = GetColorPropertyValue(ColorPropertyClass::Background);
	auto outColor = GetColorPropertyValue(ColorPropertyClass::Background_Outline);
	gr.FillRectangle(App()->GetBrushFromColorTable(bgColor), lineRect);
	gr.DrawRectangle(&Pen(App()->GetBrushFromColorTable(outColor), 1.0f), lineRect);
}

//-----------------------------------------------------------------------------

void TextFx::AlignScalePath(vector<GraphicsPath*> pathList, const RectF& lineRect,
	AlignMode alignMode)
{
	XASSERT(pathList.size() > 0);
	RectF maxBounds;
	pathList[0]->GetBounds(&maxBounds);

	for (const auto& path : pathList)
	{
		RectF bounds;
		path->GetBounds(&bounds);
		RectF::Union(maxBounds, maxBounds, bounds);
	}

	// Set alignment
	//
	Matrix vAlign;
	switch (alignMode)
	{
		case AlignMode::Center:
			vAlign.Translate(
				(lineRect.Width / 2 - maxBounds.Width / 2) - maxBounds.X, 
				(lineRect.Height / 2 - maxBounds.Height / 2) - maxBounds.Y
			);
			break;

		case AlignMode::BottomRight:
			// to-do ? 
			break;

		case AlignMode::TopLeft:
			// to-do ?
			break;
	}

	const REAL s = CalcAspectRatioToFit(maxBounds.Width, maxBounds.Height, lineRect.Width, lineRect.Height);	

	Matrix mtx;
	mtx.Translate(lineRect.Width / 2.0f, lineRect.Height / 2.0f);
	mtx.Scale(s, s);
	mtx.Translate(-lineRect.Width / 2.0f, -lineRect.Height / 2.0f);

	for (const auto& path : pathList)
	{
		path->Transform(&vAlign);
		path->Transform(&mtx);
	}
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
	auto path = unique_ptr<GraphicsPath>(ShapePath(*line.GetPath(), App()->GetCurrentShapeFunc()));

	AlignScalePath({ path.get() }, lineRect);
	DrawLineBackground(gr, lineRect);

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto faceOutline = GetColorPropertyValue(ColorPropertyClass::Face_Outline);
	
	if (!App()->GetSettings()->debugDisableFillPath)
		gr.FillPath(App()->GetBrushFromColorTable(faceColor), path.get());

	if (App()->GetSettings()->debugDrawVertices)
		DrawPathVertices(gr, *path);

	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(faceOutline), 1), path.get());
}

//-----------------------------------------------------------------------------
//
// Two Outline effect
//
//-----------------------------------------------------------------------------

void TextFxTwoOutlines::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	auto path = line.GetPathCopy();

	AlignScalePath({ path.get() }, lineRect);
	DrawLineBackground(gr, lineRect);

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto innerOutline = GetColorPropertyValue(ColorPropertyClass::Inner_Outline);
	auto outerOutline = GetColorPropertyValue(ColorPropertyClass::Outer_Outline);
	
	Pen penIn(App()->GetBrushFromColorTable(innerOutline), m_outlineWidth);
	Pen penOut(App()->GetBrushFromColorTable(outerOutline), m_outlineWidth * 2);
	const Brush* brush = App()->GetBrushFromColorTable(faceColor);
	
	gr.DrawPath(&penOut, path.get());
	gr.DrawPath(&penIn, path.get());
	gr.FillPath(brush, path.get());
}

//-----------------------------------------------------------------------------
//
// Shadow 
//
//-----------------------------------------------------------------------------

void TextFxShadow::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	auto path = line.GetPathCopy();
	auto shadowPath = line.GetPathCopy();

	RectF bounds;
	path->GetBounds(&bounds);

	/*
		Source point					Destination point
		Upper-left corner of srcRect	destPoints[0]
		Upper-right corner of srcRect	destPoints[1]
		Lower-left corner of srcRect	destPoints[2]
	*/

	const float theta = (m_shadowType == ShadowType::Rear) ? 70.0f : 250.0f;

	const REAL dx = bounds.Height * sinf(Deg2Rad(theta));
	const REAL dy = bounds.Height - (bounds.Height * cosf(Deg2Rad(theta)));

	PointF destPoints[] = {
		{ bounds.X - dx, bounds.Y + dy }, 
		{ bounds.X + bounds.Width - dx, bounds.Y + dy },	  	
		{ bounds.X, bounds.Y + bounds.Height }
	};
	
	shadowPath->Warp(destPoints, 3, bounds);

	AlignScalePath({ path.get(), shadowPath.get() }, lineRect);
	DrawLineBackground(gr, lineRect);

	// Shadow

	auto shadowColor = GetColorPropertyValue(ColorPropertyClass::Shadow);
	auto shadowOutline = GetColorPropertyValue(ColorPropertyClass::Shadow_Outline);

	if (!App()->GetSettings()->debugDisableFillPath)
		gr.FillPath(App()->GetBrushFromColorTable(shadowColor), shadowPath.get());

	if (App()->GetSettings()->debugDrawVertices)
		DrawPathVertices(gr, *shadowPath);

	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(shadowOutline), 1), shadowPath.get());

	// Face

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto faceOutline = GetColorPropertyValue(ColorPropertyClass::Face_Outline);

	if (!App()->GetSettings()->debugDisableFillPath)
		gr.FillPath(App()->GetBrushFromColorTable(faceColor), path.get());

	if (App()->GetSettings()->debugDrawVertices)
		DrawPathVertices(gr, *path);

	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(faceOutline), 1), path.get());

	// Bounding rects

	if (App()->GetSettings()->debugDrawBoundingRects)
	{
		RectF rcbPath, rcbShadow;
		path->GetBounds(&rcbPath);
		shadowPath->GetBounds(&rcbShadow);

		gr.DrawRectangle(&Pen(Color::Red, 2), rcbPath);
		gr.DrawRectangle(&Pen(Color::Green, 2), rcbShadow);
	}
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