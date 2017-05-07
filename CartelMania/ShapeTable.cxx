#include "stdafx.h"
#include "ShapeTable.h"
#include "debug.h"

using namespace std;
#define PI 3.14159265358979323846f

// Base function catalog
//
// (sin(2*PI*x)+1) / 2        Wave
// sin(PI*x)			      Arch
// (x)                        Linear
// (1-x)					  Inverted Linear
// 

void ShapeTable::Init()
{	
	dprintf(L"Initializing Shape Function Table...\n");

	m_table.push_back(std::make_pair(L"Rectangle", ShapeFunc(
		[](float x) { return 0.0f; }, 
		[](float x) { return 1.0f; })));

	m_table.push_back(std::make_pair(L"Wave", ShapeFunc(
		[](float x) { return (sinf(2 * PI * x) + 1.0f) / 2.0f; },
		[](float x) { return (sinf(2 * PI * x) + 1.0f) / 2.0f; }))); 

	m_table.push_back(std::make_pair(L"Arch", ShapeFunc(
		[](float x) { return sinf(PI * x); },
		[](float x) { return sinf(PI * x); })));
}
