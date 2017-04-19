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

void TextFxSolid::DrawLine(BannerLine& line, _In_ Graphics& gr, _In_ const RectF& lineRect)
{	
	GraphicsPath* path = line.GetPath();

	// Align at vertical and horizontal center

	RectF bounds;
	path->GetBounds(&bounds);
	
	Matrix vAlign;
	vAlign.Translate((lineRect.Width/2 - bounds.Width/2) - bounds.X, (lineRect.Height/2 - bounds.Height/2) - bounds.Y);
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

	// Draw to output device

	gr.FillPath(&SolidBrush(Gdiplus::Color::Red), path);
	gr.DrawPath(&Pen(Gdiplus::Color::Black), path);
}