#pragma once
#include <gdiplus.h>
#include <memory>
#include <vector>

using ColorTableEntry = std::pair<std::wstring, std::unique_ptr<Gdiplus::Brush>>;
using ColorTable_T = std::vector <ColorTableEntry>;

class ColorTable
{
public:
	ColorTable();
	const Gdiplus::Brush*	GetBrush(const std::wstring& name);

	size_t GetCount() const 
	{ 
		return m_table.size();  
	}
	
	const ColorTableEntry&  At(int index)
	{ 
		return m_table.at(index); 
	}

	const std::wstring& NameAt(int index)
	{
		return At(index).first;
	}

	const Gdiplus::Brush*	BrushAt(int index)
	{
		return At(index).second.get();
	}

private:
	void Init();
	ColorTable_T m_table;
};

inline Gdiplus::Color MakeRandColor() {
	return Gdiplus::Color(rand() % 255, rand() % 255, rand() % 255);
}

