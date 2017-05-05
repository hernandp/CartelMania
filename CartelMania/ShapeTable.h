#pragma once
#include <string>
#include <functional>
#include <vector>
#include "NamedTable.h"

using ShapeFn = std::function<float(float)>;

struct ShapeFunc
{
	ShapeFunc(ShapeFn top, ShapeFn bottom)
	{
		topFunc = top;
		bottomFunc = bottom;
	}

	ShapeFn topFunc, bottomFunc;
};

class ShapeTable : public NamedTable<ShapeTable, ShapeFunc>
{
public:
	void Init();
};