#include "stdafx.h"
#include "ShapeTable.h"

using namespace std;

void ShapeTable::Init()
{
	m_table.push_back(std::make_pair(L"Rectangle", ShapeFunc(
		[](float x) { return 1.0f; }, 
		[](float x) { return 0.0f; })));
}
