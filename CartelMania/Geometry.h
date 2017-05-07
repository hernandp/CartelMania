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

Gdiplus::GraphicsPath* ShapePath(const Gdiplus::GraphicsPath& path, const ShapeFunc& shapeFunc);

// ---------------------------------------------------------------------------
//
//Gdiplus::GraphicsPath* NormalizePath(const Gdiplus::GraphicsPath& path,
//	Gdiplus::REAL yMin, Gdiplus::REAL yMax, Gdiplus::REAL xMin = 0.0f,
//	Gdiplus::REAL xMax = 0.0f)
//{
//
//}

// ---------------------------------------------------------------------------

void DumpPathData(const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------

void DrawPathVertices(Gdiplus::Graphics& gr, const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------