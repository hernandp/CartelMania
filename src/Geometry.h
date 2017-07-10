/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <algorithm>
#include "ShapeTable.h"

constexpr float PI = 3.14159265358979323846f;

// ---------------------------------------------------------------------------
inline float Lerp(float a, float b, float t)
{
	return (1.0f - t) * a + t * b;
}
// ---------------------------------------------------------------------------

inline Gdiplus::REAL CalcAspectRatioToFit(float srcW, float srcH, float dstW, float dstH)
{
	return min(dstW / srcW, dstH / srcH);
}
// ---------------------------------------------------------------------------

inline constexpr float Deg2Rad(float angleDegrees)
{
	return angleDegrees * PI / 180.0f;
}

// ---------------------------------------------------------------------------

Gdiplus::REAL Length(Gdiplus::REAL x0, Gdiplus::REAL y0, 
	Gdiplus::REAL x1, Gdiplus::REAL y1);

// ---------------------------------------------------------------------------

int GetAdaptiveSubdivCount(Gdiplus::REAL x0, Gdiplus::REAL y0,
	Gdiplus::REAL x1, Gdiplus::REAL y1, Gdiplus::REAL interval);

// ---------------------------------------------------------------------------

std::vector<std::pair<Gdiplus::PointF,Gdiplus::PointF>> 
SubdivideLine(Gdiplus::REAL x0, Gdiplus::REAL y0, Gdiplus::REAL x1, Gdiplus::REAL y1, 
	int subDivCount);

// ---------------------------------------------------------------------------

void SubdivideLineAddTo(Gdiplus::REAL x0, Gdiplus::REAL y0, Gdiplus::REAL x1, Gdiplus::REAL y1, int subDivCount,
	Gdiplus::GraphicsPath& destPath);

// ---------------------------------------------------------------------------

void SubdividePath(const Gdiplus::GraphicsPath& path, Gdiplus::GraphicsPath& newPath);
// ---------------------------------------------------------------------------

Gdiplus::GraphicsPath* ShapePath(const Gdiplus::GraphicsPath& path, const ShapeFunc& shapeFunc, const Gdiplus::RectF& lineRect);

// ---------------------------------------------------------------------------

void DumpPathData(const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------

void DrawPathVertices(Gdiplus::Graphics& gr, const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------