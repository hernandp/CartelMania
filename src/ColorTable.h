#pragma once
#include <gdiplus.h>
#include <memory>
#include <vector>
#include "NamedTable.h"

using ColorTableEntry = std::pair<std::wstring, std::unique_ptr<Gdiplus::Brush>>;
using ColorTable_T = std::vector <ColorTableEntry>;

class ColorTable : public NamedTable<ColorTable, std::unique_ptr<Gdiplus::Brush>>
{
public:
	void Init();
};


inline Gdiplus::Color MakeRandColor() {
	return Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
}

