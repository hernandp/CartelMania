#pragma once
#include <string>
#include <functional>
#include <vector>
#include "NamedTable.h"

using ShapeFn_T = std::function<float(float)>;

struct ShapeFunc
{
	ShapeFunc(ShapeFn_T top, ShapeFn_T bottom)
	{
		topFunc = top;
		bottomFunc = bottom;
	}

	ShapeFn_T topFunc, bottomFunc;
};

class ShapeTable : public NamedTable<ShapeTable, ShapeFunc>
{
public:
	void Init();
};