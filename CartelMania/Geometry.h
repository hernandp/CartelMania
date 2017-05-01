#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>


inline constexpr float Deg2Rad(float angleDegrees)
{
	return angleDegrees * 3.14159265358979323846f  / 180.0f;
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

Gdiplus::GraphicsPath* WarpPath(const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------

void DumpPathData(const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------

void DrawPathVertices(Gdiplus::Graphics& gr, const Gdiplus::GraphicsPath& path);

// ---------------------------------------------------------------------------