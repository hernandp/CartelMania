#include "textrend.h"
#include <windows.h>
#include <gdiplus.h>
#include "textrend.h"
#include "textutil.h"
#include "banner.h"
#include "colors.h"
#include "Debug.h"
#include "geom.h"

using namespace Gdiplus;
using namespace std;
using namespace Cartelmania;

void TextFxSolid::DrawLine(const BannerLine& line, Graphics& gr, const RectF& lineRect)
{
	StringFormat strFormat;
	strFormat.SetTrimming(StringTrimmingNone);
	strFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	strFormat.SetAlignment(StringAlignmentCenter);

	RectF boundingBox;
	FontFamily family(line.GetFontName().c_str());
	unique_ptr<Font> font = FindFontToFillTextInRect(gr, lineRect, family, line.GetText(), strFormat, &boundingBox);

	auto fontSize = PointToPixels(gr, font->GetSize());
	auto fontStyle = font->GetStyle();
	Gdiplus::FontFamily fontFamily;
	font->GetFamily(&fontFamily);

	Gdiplus::GraphicsPath path;
	Gdiplus::Pen pen(g_Colors[3].m_fg, m_outlineWidth);
	Gdiplus::HatchBrush brush(g_Colors[2].m_hatch, g_Colors[2].m_fg, g_Colors[2].m_bg);
	path.AddString(line.GetText().c_str(), -1, &fontFamily, fontStyle, fontSize, lineRect, &strFormat);

	// The measured rect passed to AddString accounts for font descent, even if all chars in the string
	// are upon the baseline. We scale the enclosing bounding box of the path to fill the drawing area,
	// minus some margin.
	//

	// Align at center

	RectF bounds;
	path.GetBounds(&bounds, nullptr, &pen);
	Matrix vAlign;
	vAlign.Translate(0, (lineRect.Height/2 - bounds.Height/2) - bounds.Y);
	path.Transform(&vAlign);

	XASSERT(lineRect.Contains(bounds));

	// Calculate how much X/Y-scaling we need.
	// Mantain aspect-ratio (sX=sY)
	
	const REAL ratio = bounds.Width / bounds.Height;
	const REAL s = lineRect.Height / bounds.Height;

	Matrix mtx;
	mtx.Translate(lineRect.Width / 2.0f, lineRect.Height / 2.0f);
	mtx.Scale(s, s);
	mtx.Translate(-lineRect.Width / 2.0f, -lineRect.Height / 2.0f);
	path.Transform(&mtx);

	path.Flatten();

	PathData pathData;
	path.GetPathData(&pathData);
	Gdiplus::GraphicsPath newPath;

	////////-------
	Gdiplus::GraphicsPathIterator pathIter(&path);
	pathIter.Rewind();
	dprintf(L"String Path subpath count=%d\n", pathIter.GetSubpathCount());
	
	for (int i = 0; i < pathIter.GetSubpathCount(); ++i)
	{
		Gdiplus::GraphicsPath charPath;
		BOOL isClosed;
		int dataSize = pathIter.NextSubpath(&charPath, &isClosed);

		dprintf(L">> subpath [%d], closed=%d, data_points=%d\n", i, isClosed, dataSize);

		for (int j = 0; j < dataSize - 1; j++)
		{
			const REAL px0 = pathData.Points[j].X;
			const REAL py0 = pathData.Points[j].Y;
			const REAL px1 = pathData.Points[j + 1].X;
			const REAL py1 = pathData.Points[j + 1].Y;
			//const REAL dist = sqrtf((px1 - px0)*(px1 - px0) + (py1 - py0)*(py1 - py0));

			SubdivideLineAddTo(px0, py0, px1, py1, 2, newPath, true, &gr);
		}

		// Close path

		SubdivideLineAddTo(pathData.Points[dataSize-1].X, pathData.Points[dataSize-1].Y, 
			pathData.Points[0].X, pathData.Points[0].Y, 2, newPath, true, &gr);
	}


	//GraphicsPath* charPath = nullptr;
	//BOOL isClosed;


	//while (pathIter.NextSubpath(&path, &isClosed) > 0)
	//{
	//	XASSERT(isClosed); 
	//	PathData pathData;
	//	charPath->GetPathData(&pathData);

	//	dprintf(L"Path point# =%d\n", pathData.Count);
	//}*/
	//
	//dprintf(L"Path point# =%d\n", pathData.Count);
	//for (int i = 0; i < pathData.Count; ++i)
	//{
	//	const REAL px0 = pathData.Points[i].X;
	//	const REAL py0 = pathData.Points[i].Y;
	//	const REAL px1 = pathData.Points[i+1].X;
	//	const REAL py1 = pathData.Points[i+1].Y;
	//	const REAL dist = sqrtf((px1 - px0)*(px1 - px0) + (py1 - py0)*(py1 - py0));
	//	
	//	const REAL  SUBDIV = 2.0f;
	//	REAL px = px0, py = py1;		

	//	// Cx = Ax * (1 - t) + Bx * t
	//	// Cy = Ay * (1 - t) + By * t

	//	dprintf(L"(PX0=%.f PY0=%.f)-(PX1=%.f PY1=%.f) \n", px0, py0, px1, py1);

	//	for (int j = 0; j < (int)SUBDIV ; j++)
	//	{
	//		const REAL t = j / SUBDIV;
	//		REAL nx = px0 * (1 - t) + px1 * t;
	//		REAL ny = py0 * (1 - t) + py1 * t;
	//		newPath.AddLine(px, py, nx, ny);
	//		gr.DrawLine(&Pen(MakeRandColor(), 2), px, py, nx, ny);
	//		px = nx;
	//		py = ny;
	//	}

	//	if (pathData.Types[i+1] & 0x80)
	//	{
	//		// End  of shape
	//		newPath.AddLine(px, py, px0, py0);
	//		gr.DrawLine(&Pen(MakeRandColor(), 2), px, py, px0, py0);
	//		gr.FillEllipse(&SolidBrush(Gdiplus::Color::Blue), pathData.Points[i].X - 5, pathData.Points[i].Y - 5, 10.0f, 10.0f);
	//	}
	//	else
	//	{
	//		newPath.AddLine(px, py, px1, py1);
	//		gr.DrawLine(&Pen(MakeRandColor(), 2), px, py, px1, py1);
	//	}

	//	//else
	//	//{
	//	//	gr.DrawEllipse(&Pen(Gdiplus::Color::Red, 2), pathData.Points[i].X - 5, pathData.Points[i].Y - 5, 10.0f, 10.0f);
	//	//	gr.DrawLine(&Pen(Gdiplus::Color::Green, 2), pathData.Points[i], pathData.Points[i + 1]);
	//	//}

	//}

	//gr.FillPath(&brush, &newPath);
	//gr.DrawPath(&pen, &newPath);
}