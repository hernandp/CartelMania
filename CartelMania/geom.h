#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <vector>

inline std::vector<std::pair<Gdiplus::PointF,Gdiplus::PointF>> 
SubdivideLine(Gdiplus::REAL x0, Gdiplus::REAL y0, Gdiplus::REAL x1, Gdiplus::REAL y1, int subDivCount)
{
	std::vector<std::pair<Gdiplus::PointF, Gdiplus::PointF>> v(64);
	const Gdiplus::REAL  SUBDIV = (Gdiplus::REAL)subDivCount;
	Gdiplus::REAL px = x0, py = y0;

	dprintf(L"original (PX0=%.f PY0=%.f)-(PX1=%.f PY1=%.f) \n" ,x0, y0, x1, y1);

	for (int k = 1; k < (int) SUBDIV + 1; k++)
	{
		// Cx = Ax * (1 - t) + Bx * t
		// Cy = Ay * (1 - t) + By * t

		const Gdiplus::REAL t = k / SUBDIV;
		Gdiplus::REAL nx = x0 * (1 - t) + x1 * t;
		Gdiplus::REAL ny = y0 * (1 - t) + y1 * t;

		dprintf(L"subdiv line k=%d (PX0=%.f PY0=%.f)-(PX1=%.f PY1=%.f) \n", k, px, py, nx, ny);
		v.push_back(std::make_pair(Gdiplus::PointF(px, py), Gdiplus::PointF(nx, ny)));
		px = nx;
		py = ny;
	}

	return v;
}

inline void SubdivideLineAddTo(Gdiplus::REAL x0, Gdiplus::REAL y0, Gdiplus::REAL x1, Gdiplus::REAL y1, int subDivCount,
	Gdiplus::GraphicsPath& destPath, bool fDebugDraw = false, Gdiplus::Graphics* gr = nullptr)
{
	auto segments = SubdivideLine(x0, y0, x1, y1, subDivCount);
	for (const auto& seg : segments)
	{
		if (fDebugDraw)
		{
			XASSERT(gr != nullptr);
			gr->DrawLine(&Gdiplus::Pen(MakeRandColor(), 2), seg.first, seg.second);
		}
		destPath.AddLine(seg.first, seg.second);
	}
}