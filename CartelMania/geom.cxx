#include "geom.h"
#include <windows.h>
#include <gdiplus.h>
#include "debug.h"
#include "colors.h"
#include <algorithm>
#include <memory>

using namespace Gdiplus;
using namespace std;

// ---------------------------------------------------------------------------

REAL Length(REAL x0, REAL y0, REAL x1, REAL y1)
{
	return sqrtf((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
}

int GetAdaptiveSubdivCount(Gdiplus::REAL x0, Gdiplus::REAL y0, Gdiplus::REAL x1, Gdiplus::REAL y1, Gdiplus::REAL interval)
{
	const REAL iv = Length(x0, y0, x1, y1) / interval;
	return (iv < 1.0f) ? 1 : static_cast<int>(iv);
}

// ---------------------------------------------------------------------------

vector<pair<PointF, PointF>> SubdivideLine(REAL x0, REAL y0, REAL x1, REAL y1, 
	int subDivCount)
{
	vector< pair< PointF, PointF>> v;
	const  REAL  SUBDIV = (REAL) subDivCount;
	REAL px = x0, py = y0;

	dprintf(L"original (PX0=%.f PY0=%.f)-(PX1=%.f PY1=%.f) \n", x0, y0, x1, y1);

	for (int k = 1; k < (int) SUBDIV + 1; k++)
	{
		// Cx = Ax * (1 - t) + Bx * t
		// Cy = Ay * (1 - t) + By * t

		const  REAL t = k / SUBDIV;
		REAL nx = x0 * (1 - t) + x1 * t;
		REAL ny = y0 * (1 - t) + y1 * t;

		dprintf(L"subdiv line k=%d (PX0=%.f PY0=%.f)-(PX1=%.f PY1=%.f) \n", k, px, py, nx, ny);
		v.push_back(make_pair(PointF(px, py), PointF(nx, ny)));
		px = nx;
		py = ny;
	}

	return v;
}

// ---------------------------------------------------------------------------

void SubdivideLineAddTo(REAL x0, REAL y0, REAL x1, REAL y1, 
	int subDivCount, GraphicsPath& destPath, bool fDebugDraw, Graphics* gr)
{
	auto segments = SubdivideLine(x0, y0, x1, y1, subDivCount);
	for (const auto& seg : segments)
	{
		destPath.AddLine(seg.first, seg.second);
	}
}

// ---------------------------------------------------------------------------

void SubdividePath(const GraphicsPath& path, GraphicsPath& newPath)
{
	int lastStartIndex = -1;
	PathData pathData;
	path.GetPathData(&pathData);
	for (int i = 1; i < pathData.Count; ++i)
	{
		if (pathData.Types[i-1] == 0)
		{
			lastStartIndex = i-1;
			newPath.StartFigure();
		}

		const REAL px0 = pathData.Points[i - 1].X;
		const REAL py0 = pathData.Points[i - 1].Y;
		const REAL px1 = pathData.Points[i].X;
		const REAL py1 = pathData.Points[i].Y;

		const int subdivCount = GetAdaptiveSubdivCount(px0, py0, px1, py1, 16.0f);
		
		SubdivideLineAddTo(px0, py0, px1, py1, subdivCount, newPath);

		if (pathData.Types[i] & 0x20) // marker?
			newPath.SetMarker();

		if (pathData.Types[i] & 0x80) // end of figure
		{
			// If we let GDI+ to close figure, he'll trace a single segment to
			// the figure origin; we add a subdivided line to the origin by ourselves,
			// but keep the last segment out, to let GDI+ to close the figure 
			// as in the "normal" behavior.
			// 

			XASSERT(lastStartIndex != -1);
			const REAL x0 = pathData.Points[i].X;
			const REAL y0 = pathData.Points[i].Y;
			const REAL x1 = pathData.Points[lastStartIndex].X;
			const REAL y1 = pathData.Points[lastStartIndex].Y;

			const int subdivCount = GetAdaptiveSubdivCount(x0, y0, x1, y1, 16.0f);

			auto lines = SubdivideLine(x0, y0, x1, y1,	subdivCount);

			lines.pop_back();
			for (const auto& line : lines)
				newPath.AddLine(line.first, line.second);

			newPath.CloseFigure();
			i++;
		}
	}
}

// ---------------------------------------------------------------------------

GraphicsPath* WarpPath(const GraphicsPath& path)
{
	PathData pd;
	path.GetPathData(&pd);

	/*for (int i = 0; i < pd.Count; ++i)
	{
		pd.Points[i].Y += 10.0f * sinf(float(i));
	}	*/

	return new GraphicsPath(pd.Points, pd.Types, pd.Count);
}

// ---------------------------------------------------------------------------

void DumpPathData(const  GraphicsPath& path)
{
#ifdef _DEBUG
	PathData pathData;
	path.GetPathData(&pathData);

	for (int i = 0; i < pathData.Count; i++)
	{
		dprintf(L"POINT %d TYPE=0x%02x X=%.f Y=%.f\n", i, pathData.Types[i], pathData.Points[i].X, pathData.Points[i].Y);
	}
#endif
}

// ---------------------------------------------------------------------------

void DrawPathVertices(Graphics& gr, const GraphicsPath& path)
{
	//gr.DrawPath(&Pen(Color::Black, 2), &path);
	
	PathData pd;
	path.GetPathData(&pd);

	for (int i = 0; i < pd.Count; ++i)
	{
		BYTE type = pd.Types[i];
		PointF pt = pd.Points[i];

		if (type & 0x80) // End
		{
			gr.FillRectangle(&SolidBrush(Color::Blue), RectF(pt.X - 5.0f, pt.Y - 5.0f, 10.0f, 10.0f));
		}

		if (type & 0x20) // Marker
		{
			gr.DrawEllipse(&Pen(Color::Green), RectF(pt.X - 5.0f, pt.Y - 5.0f, 10.0f, 10.0f));
		}

		gr.FillRectangle(&SolidBrush(Color::Red), RectF(pt.X - 2.5f, pt.Y - 2.5f, 5.0f, 5.0f));
	}	
}

// ---------------------------------------------------------------------------