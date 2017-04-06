#pragma once
#include <gdiplus.h>
#include <memory>

using BrushPtr = std::unique_ptr<Gdiplus::Brush>;

class Colorset
{
	Colorset(BrushPtr background,
		BrushPtr backgroundOutline,
		BrushPtr face,
		BrushPtr faceOutline,
		BrushPtr shadow,
		BrushPtr shadowOutline
	)
		:
		m_background(std::move(background)),
		m_backgroundOutline(std::move(backgroundOutline)),
		m_face(std::move(face)),
		m_faceOutline(std::move(faceOutline)),
		m_shadow(std::move(shadow)),
		m_shadowOutline(std::move(shadowOutline))
	{	};

	Colorset(Colorset&) = delete;

	const Gdiplus::Brush& GetBackground() const { return *m_background.get(); }
	const Gdiplus::Brush& GetBackgroundOutline() const { return *m_backgroundOutline.get(); }
	const Gdiplus::Brush& GetFace() const { return *m_face.get(); }
	const Gdiplus::Brush& GetFaceOutline() const { return *m_faceOutline.get(); }
	const Gdiplus::Brush& GetShadow() const { return *m_shadow.get(); }
	const Gdiplus::Brush& GetShadowOutline() const { return *m_shadowOutline.get(); }

private:
	BrushPtr	m_background;
	BrushPtr	m_backgroundOutline;
	BrushPtr	m_face;
	BrushPtr	m_faceOutline;
	BrushPtr	m_shadow;
	BrushPtr	m_shadowOutline;
};