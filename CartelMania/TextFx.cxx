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
	Matrix mScale;
	switch (App()->GetBanner()->GetScalePolicy())
	{
		case  ScalePolicy::KeepAspect:
		
			break;

		case ScalePolicy::ScaleToFit:
			mScale.Translate(lineRect.Width / 2.0f, lineRect.Height / 2.0f);
			mScale.Scale(lineRect.Width / maxBounds.Width, lineRect.Height / maxBounds.Height);
			mScale.Translate(-lineRect.Width / 2.0f, -lineRect.Height / 2.0f);
			break;
	}

	const REAL s = CalcAspectRatioToFit(maxBounds.Width, maxBounds.Height, lineRect.Width, lineRect.Height);	

	for (const auto& path : pathList)
	{
		path->Transform(&vAlign);
		path->Transform(&mScale);
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

void TextFxSolid::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
    auto path = line.GetPathCopy();
	
	path = unique_ptr<GraphicsPath>(ShapePath(*line.GetPath(), App()->GetCurrentShapeFunc(), lineRect));
	AlignScalePath({ path.get() }, lineRect);

	// Bounding rects
		
	DrawLineBackground(gr, lineRect);

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto faceOutline = GetColorPropertyValue(ColorPropertyClass::Face_Outline);
	
	if (!App()->GetSettings()->debugDisableFillPath)
		gr.FillPath(App()->GetBrushFromColorTable(faceColor), path.get());

	if (App()->GetSettings()->debugDrawVertices)
		DrawPathVertices(gr, *path);

	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(faceOutline), 1), path.get());

	if (App()->GetSettings()->debugDrawBoundingRects)
	{
		RectF rcbPath;
		path->GetBounds(&rcbPath);

		gr.DrawRectangle(&Pen(Color::Red, 2), rcbPath);
	}
}

//-----------------------------------------------------------------------------
//
// Two Outline effect
//
//-----------------------------------------------------------------------------

void TextFxTwoOutlines::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	auto path = line.GetPathCopy();

	path = unique_ptr<GraphicsPath>(ShapePath(*line.GetPath(), App()->GetCurrentShapeFunc(), lineRect));
	AlignScalePath({ path.get() }, lineRect);

	// Bounding rects

	DrawLineBackground(gr, lineRect);

	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto innerOutline = GetColorPropertyValue(ColorPropertyClass::Inner_Outline);
	auto outerOutline = GetColorPropertyValue(ColorPropertyClass::Outer_Outline);
	
	// TODO: Adjustable outline sizes
	//
	Pen penIn(App()->GetBrushFromColorTable(innerOutline), 8);
	Pen penOut(App()->GetBrushFromColorTable(outerOutline), 4);
	const Brush* brush = App()->GetBrushFromColorTable(faceColor);
	penIn.SetAlignment(PenAlignmentInset);
	penOut.SetAlignment(PenAlignmentInset);
	gr.FillPath(brush, path.get());
	gr.DrawPath(&penIn, path.get());
	gr.DrawPath(&penOut, path.get());
	
}

//-----------------------------------------------------------------------------
// 
// Shadow 
//
//-----------------------------------------------------------------------------

void TextFxShadow::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	auto path = line.GetPathCopy();
	path = unique_ptr<GraphicsPath>(ShapePath(*path.get(), App()->GetCurrentShapeFunc(), lineRect));
	auto shadowPath = line.GetPathCopy();
	shadowPath = unique_ptr<GraphicsPath>(ShapePath(*shadowPath.get(), App()->GetCurrentShapeFunc(), lineRect));

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
	const int BLOCKDEPTH = 16, NUMBLOCKS = 6;
	auto path = line.GetPathCopy();
	auto backPath = line.GetPathCopy();
	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto faceOutline = GetColorPropertyValue(ColorPropertyClass::Face_Outline);
	
	path = unique_ptr<GraphicsPath>(ShapePath(*line.GetPath(), App()->GetCurrentShapeFunc(), lineRect));
	backPath = unique_ptr<GraphicsPath>(ShapePath(*line.GetPath(), App()->GetCurrentShapeFunc(), lineRect));

	DrawLineBackground(gr, lineRect);

	Matrix mtx;
	switch (m_blockDir)
	{
		case BlockDirection::TopLeft:
			mtx.Translate(-BLOCKDEPTH * NUMBLOCKS, -BLOCKDEPTH * NUMBLOCKS);
			break;

		case BlockDirection::TopRight:
			mtx.Translate(BLOCKDEPTH * NUMBLOCKS, -BLOCKDEPTH * NUMBLOCKS);
			break;

		case BlockDirection::BottomLeft:
			mtx.Translate(-BLOCKDEPTH * NUMBLOCKS, BLOCKDEPTH * NUMBLOCKS);
			break;

		case BlockDirection::BottomRight:
			mtx.Translate(BLOCKDEPTH * NUMBLOCKS, BLOCKDEPTH * NUMBLOCKS);
			break;
	}
	
	backPath->Transform(&mtx);	
	
	AlignScalePath({ path.get(), backPath.get() }, lineRect);
	
	RectF backPathBounds, pathBounds;
	backPath->GetBounds(&backPathBounds);
	path->GetBounds(&pathBounds);
	
	float slope = (pathBounds.Y - backPathBounds.Y) / (pathBounds.X - backPathBounds.X);

	int xDist = (int) fabs(backPathBounds.X - pathBounds.X);
	std::wstring blockColor;

	for (int cx = 0; cx < xDist; cx++)
	{
		switch (int(NUMBLOCKS * (float(cx) / xDist)))
		{
			case 0: blockColor = GetColorPropertyValue(ColorPropertyClass::Shade_1); break;
			case 1: blockColor = GetColorPropertyValue(ColorPropertyClass::Shade_2); break;
			case 2: blockColor = GetColorPropertyValue(ColorPropertyClass::Shade_3); break;
			case 3: blockColor = GetColorPropertyValue(ColorPropertyClass::Shade_4); break;
			case 4: blockColor = GetColorPropertyValue(ColorPropertyClass::Shade_5); break;
			case 5: blockColor = GetColorPropertyValue(ColorPropertyClass::Shade_6); break;
		}

		mtx.Reset();
		if (m_blockDir == BlockDirection::TopLeft || m_blockDir == BlockDirection::BottomLeft)
		{
			mtx.Translate((float) cx, slope * (float) cx);
		}
		else // Top-right & bottom-right
		{
			mtx.Translate((float) -cx, -slope * (float) cx);
		}
			
		backPath->Transform(&mtx);
		gr.FillPath(App()->GetBrushFromColorTable(blockColor), backPath.get());
		mtx.Invert();
		backPath->Transform(&mtx);	
	}

	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(faceOutline), 1), path.get());
	gr.FillPath(App()->GetBrushFromColorTable(faceColor), path.get());
}


//-----------------------------------------------------------------------------

void TextFxPerspective::DrawLine(BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	auto path = line.GetPathCopy();
	auto backPath = line.GetPathCopy();
	auto faceColor = GetColorPropertyValue(ColorPropertyClass::Face);
	auto faceOutline = GetColorPropertyValue(ColorPropertyClass::Face_Outline);
	auto sideColor = GetColorPropertyValue(ColorPropertyClass::Sides);
	const float YOFFSET = 180;

	DrawLineBackground(gr, lineRect);

	RectF backPathBounds, pathBounds;
	backPath->GetBounds(&backPathBounds);
	path->GetBounds(&pathBounds);

	Matrix mtx;
	mtx.Scale(0.5, 0.5);

	switch (m_direction)
	{
		case PerspectiveDirection::Down:
			//mtx.Translate(pathBounds.Width / 2.0f, YOFFSET * 2);
			break;

		case PerspectiveDirection::Up:
			mtx.Translate(pathBounds.Width / 2.0f, -YOFFSET);
			break;
	}
	backPath->Transform(&mtx);

	//path = unique_ptr<GraphicsPath>(ShapePath(*line.GetPath(), App()->GetCurrentShapeFunc(), lineRect));

	AlignScalePath({ path.get(), backPath.get() }, lineRect);

	backPath->GetBounds(&backPathBounds);
	path->GetBounds(&pathBounds);

	int yDist = (int) fabs(backPathBounds.Y - pathBounds.Y);

	for (int cy = 0; cy < yDist; cy++)
	{
		/*	mtx.Reset();
			mtx.Scale(1.0f + (0.5f * ((float) cy / yDist)), 1.0f + (0.5f * ((float) cy / yDist)));
			backPath->GetBounds(&backPathBounds);
			mtx.Translate( 0.0f , (float) cy);
			backPath->Transform(&mtx);
			gr.FillPath(App()->GetBrushFromColorTable(sideColor), backPath.get());
			mtx.Invert();
			backPath->Transform(&mtx);*/
	}

	

	//float slope = (pathBounds.Y - backPathBounds.Y) / (pathBounds.X - backPathBounds.X);

	//std::wstring blockColor;

	//for (int cx = 0; cx < xDist; cx++)
	//{
	//	mtx.Reset();
	//	if (m_direction == PerspectiveDirection::Down)
	//	{
	//		mtx.Translate((float) cx, slope * (float) cx);
	//	}
	//	else // Top-right & bottom-right
	//	{
	//		mtx.Translate((float) -cx, -slope * (float) cx);
	//	}

	//	backPath->Transform(&mtx);
	//	gr.FillPath(App()->GetBrushFromColorTable(blockColor), backPath.get());
	//	mtx.Invert();
	//	backPath->Transform(&mtx);
	//}


	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(faceOutline), 1), backPath.get());
	gr.DrawPath(&Pen(App()->GetBrushFromColorTable(faceOutline), 1), path.get());
	gr.FillPath(App()->GetBrushFromColorTable(faceColor), path.get());
}

//-----------------------------------------------------------------------------

void TextFxBlend::DrawLine(BannerLine& line, Graphics& gr, const RectF& bannerRect)
{

}