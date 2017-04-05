#include "banner.h"
#include "textrend.h"
#include <gdiplus.h>

using namespace Gdiplus;

Banner::Banner() : m_fontName(L"Arial"), m_textRenderer(new TextFxSolid)
{

}

Banner::~Banner()
{
	if (m_textRenderer)
		delete m_textRenderer;
}

void Banner::SetTextRenderer(ITextFXRenderer * textRenderer)
{
	if (m_textRenderer)
		delete m_textRenderer;

	m_textRenderer = textRenderer;
}

void Banner::PaintOn(HDC hdc, const LPRECT rcClient)
{
	Graphics gr(hdc);

	m_text[0] = L"\u263ACARTELMANIA\u263A";
	
	RectF rc;
	rc.X = (REAL)rcClient->left;
	rc.Y = (REAL)rcClient->top;
	rc.Width = (REAL) (rcClient->right - rcClient->left);
	rc.Height = (REAL) (rcClient->bottom - rcClient->top);

	// Draw background
	//

	gr.FillRectangle(&HatchBrush(HatchStyle10Percent, Color::Blue, Color::DarkCyan), rc);
	
	//
	// Let's fill the banner rectangle. Keep font proportional,
	// but set it's height to near 60% (?) the enclosing rectangle.
	//

	rc.Inflate(0, -rc.Height * 0.20f);

	gr.FillRectangle(&SolidBrush(Color::White), rc);
	gr.DrawRectangle(&Pen(Color::Black), rc);

	m_textRenderer->Draw(*this, gr, rc);
}