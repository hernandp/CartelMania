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
#include "stdafx.h"
#include "EffectTable.h"
#include "debug.h"

using namespace std;

void EffectTable::Init()
{
	//
	// I18N Warning: Always keep English names stored HERE AS-IS, as they are
	//               referenced in saved XML files (see Banner::Serialize method). I18N must be done
	//               at 'UI level'.
	//

	dprintf(L"Initializing Effect Table...\n");

	m_table.push_back(make_pair(L"Solid",				[] { return make_unique<TextFxSolid>(); }));
	m_table.push_back(make_pair(L"Two Outlines",		[] { return make_unique<TextFxTwoOutlines>(); }));
	m_table.push_back(make_pair(L"Rear Shadow",			[] { return make_unique<TextFxShadow>(ShadowType::Rear); }));
	m_table.push_back(make_pair(L"Fore Shadow",			[] { return make_unique<TextFxShadow>(ShadowType::Fore); }));
	m_table.push_back(make_pair(L"Block Top-Left",		[] { return make_unique<TextFxBlock>(BlockDirection::TopLeft);	}));
	m_table.push_back(make_pair(L"Block Top-Right",		[] { return make_unique<TextFxBlock>(BlockDirection::TopRight);	}));
	m_table.push_back(make_pair(L"Block Bottom-Left",	[] { return make_unique<TextFxBlock>(BlockDirection::BottomLeft);	}));
	m_table.push_back(make_pair(L"Block Bottom-Right",	[] { return make_unique<TextFxBlock>(BlockDirection::BottomRight);	}));
	//m_table.push_back(make_pair(L"Perspective Up",		[] { return make_unique<TextFxPerspective>(PerspectiveDirection::Up);	}));
	//m_table.push_back(make_pair(L"Perspective Down",	[] { return make_unique<TextFxPerspective>(PerspectiveDirection::Down);	}));
}