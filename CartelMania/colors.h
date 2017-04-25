#pragma once
#include <gdiplus.h>
#include <memory>
#include <vector>

using ColorTable_T = std::vector < std::pair<std::wstring, std::unique_ptr<Gdiplus::Brush>>>;

// ---------------------------------------------------------------------------
// 
void						InitColorTable();
const Gdiplus::Brush*		GetBrushFromColorTable(const std::wstring& name);
Gdiplus::Color				MakeRandColor();

extern ColorTable_T	g_colorTable;

// ---------------------------------------------------------------------------
