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
	//
	// I18N Warning: Always keep English names stored HERE AS-IS, as they are
	//               referenced in saved XML files (see Banner::Serialize method). I18N must be done
	//               at 'UI level'.
	//

	dprintf(L"Initializing Shape Function Table...\n");

	m_table.push_back(std::make_pair(L"Rectangle", ShapeFunc(
		[](float x) { return 0.0f; }, 
		[](float x) { return 1.0f; })));

	m_table.push_back(std::make_pair(L"Right Pennant", ShapeFunc(
		[](float x) { return x; },
		[](float x) { return 1.0f -x; })));

	m_table.push_back(std::make_pair(L"Left Pennant", ShapeFunc(
		[](float x) { return 1.0f - x; },
		[](float x) { return x; })));

	m_table.push_back(std::make_pair(L"Right Flag", ShapeFunc(
		[](float x) { return x/1.5f; },
		[](float x) { return 1.0f - x/1.5f; })));

	m_table.push_back(std::make_pair(L"Left Flag", ShapeFunc(
		[](float x) { return (2.0f/3.0f) - x/1.5f; },
		[](float x) { return 1.0f - ((2.0f/3.0f) - x/1.5f); })));

	m_table.push_back(std::make_pair(L"Slash Up", ShapeFunc(
		[](float x) { return -x; },
		[](float x) { return -x; })));

	m_table.push_back(std::make_pair(L"Slash Down", ShapeFunc(
		[](float x) { return x; },
		[](float x) { return x; })));

	m_table.push_back(std::make_pair(L"Taper Up", ShapeFunc(
		[](float x) { return 0.0f; },
		[](float x) { return 1-sqrtf(x); })));

	m_table.push_back(std::make_pair(L"Taper Down", ShapeFunc(
		[](float x) { return 0.0f; },
		[](float x) { return x*x; })));

	m_table.push_back(std::make_pair(L"Taper Left", ShapeFunc(
		[](float x) { return 1.0f - x; },
		[](float x) { return x; })));

	m_table.push_back(std::make_pair(L"Taper Right", ShapeFunc(
		[](float x) { return sqrtf(x); },
		[](float x) { return x*x; })));

	m_table.push_back(std::make_pair(L"Arch Down", ShapeFunc(
		[](float x) { return sinf(PI * x); },
		[](float x) { return sinf(PI * x); })));

	m_table.push_back(std::make_pair(L"Arch Up", ShapeFunc(
		[](float x) { return -sinf(PI * x); },
		[](float x) { return -sinf(PI * x); })));

	m_table.push_back(std::make_pair(L"Advancing Wave", ShapeFunc(
		[](float x) { return (sinf(2 * PI * x) + 1.0f) / 2.0f; },
		[](float x) { return (sinf(2 * PI * x) + 1.0f) / 2.0f; })));

	m_table.push_back(std::make_pair(L"Receding Wave", ShapeFunc(
		[](float x) { return 1.0f-(sinf(2 * PI * x) + 1.0f) / 2.0f; },
		[](float x) { return 1.0f-(sinf(2 * PI * x) + 1.0f) / 2.0f; })));

	m_table.push_back(std::make_pair(L"Ribbon", ShapeFunc(
		[](float x) { return (x <= 0.5f) ? -sinf(PI * x) : sinf(PI * x) - 2.0f; },
		[](float x) { return (x <= 0.5f) ? -sinf(PI * x) : sinf(PI * x) - 2.0f; })));

}
