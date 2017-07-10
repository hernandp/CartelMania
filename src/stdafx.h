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
//
// Precompiled headers
//

// Windows headers
//
#include <windows.h>
#include <WindowsX.h>
#include <gdiplus.h>

// ATL/WTL headers
//
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlprint.h>

// C/C++ headers
//
#include <algorithm>
#include <memory>
#include <cassert>
#include <map>
#include <array>
#include <limits>
#include <stdexcept>
#include <fstream>
#include <set>

// PugiXML
#define PUGIXML_WCHAR_MODE 
#include <../../pugixml/pugixml.hpp>