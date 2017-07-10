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

// Global program configuration
//

const int DEFAULT_PATH_SUBDIV_INTERVAL =64;
const int DEFAULT_MAX_TEXT_LENGTH = 256;

class AppSettings
{
public:
	DWORD Save();
	DWORD Load();

	bool debugDisableSubdiv = false;
	bool debugDrawVertices = false;
	bool debugDisableFillPath = false;
	bool debugDrawBoundingRects = false;
	int pathSubDivInterval = DEFAULT_PATH_SUBDIV_INTERVAL;
	int maxTextLength = DEFAULT_MAX_TEXT_LENGTH;
	DWORD debugLog = 0;

	POINT lastTextEditToolPos = { -1,-1 };
	POINT lastColorEditToolPos = { -1,-1 };
	POINT lastShapeEditToolPos = { -1,-1 };
	POINT lastShapeEditToolSize = { -1,-1 };
	POINT lastLayoutEditToolPos = { -1,-1 };
	POINT lastLineLayoutToolPos = { -1, -1 };
	POINT lastLineLayoutToolSize = { -1, -1 };
	POINT lastEffectToolPos = { -1, -1 };
	POINT lastEffectToolSize = { -1, -1 };

private:
	LSTATUS PackDWORDs(CRegKey& k, const wchar_t* valueName, DWORD dwHi, DWORD dwLo);

	template <class T>
	LSTATUS UnpackDWORDs(CRegKey& k, const wchar_t* valueName, T& dwHi, T& dwLo);
};
