/*****************************************************************************

    CartelMania
	Copyright(C) 2017 Hernán Di Pietro

	This program is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < http://www.gnu.org/licenses/>.

*****************************************************************************/
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