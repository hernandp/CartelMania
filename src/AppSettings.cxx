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
#include "AppSettings.h"

LSTATUS AppSettings::PackDWORDs(CRegKey& k, const wchar_t* valueName, DWORD dwHi, DWORD dwLo)
{
	return k.SetQWORDValue(valueName, ((UINT64) dwHi << 32) | dwLo);
}

template <class T>
LSTATUS AppSettings::UnpackDWORDs(CRegKey& k, const wchar_t* valueName, T& hi, T& lo)
{
	ULONGLONG ul;
	LSTATUS r = k.QueryQWORDValue(valueName, ul);

	if (r == ERROR_SUCCESS)
	{
		hi = static_cast<T>((ul >> 32) & 0x00000000ffffffff);
		lo = static_cast<T>(ul & 0x00000000ffffffff);
	}
	return r;
}

DWORD AppSettings::Save()
{
	CRegKey regKey;
	LRESULT lr = regKey.Create(HKEY_CURRENT_USER, L"SOFTWARE\\CartelMania");
	if (lr == ERROR_SUCCESS)
	{
		regKey.SetDWORDValue(L"debugLog", debugLog);
		PackDWORDs(regKey, L"lastTextEditToolPos", lastTextEditToolPos.x, lastTextEditToolPos.y);
		PackDWORDs(regKey, L"lastColorEditToolPos", lastColorEditToolPos.x, lastColorEditToolPos.y);
		PackDWORDs(regKey, L"lastShapeEditToolPos", lastShapeEditToolPos.x, lastShapeEditToolPos.y);
		PackDWORDs(regKey, L"lastShapeEditToolSize", lastShapeEditToolSize.x, lastShapeEditToolSize.y);
		PackDWORDs(regKey, L"lastLayoutEditToolPos", lastLayoutEditToolPos.x, lastLayoutEditToolPos.y);
		PackDWORDs(regKey, L"lastLineLayoutToolPos", lastLineLayoutToolPos.x, lastLineLayoutToolPos.y);
		PackDWORDs(regKey, L"lastLineLayoutToolSize", lastLineLayoutToolSize.x, lastLineLayoutToolSize.y);
		PackDWORDs(regKey, L"lastEffectToolPos", lastEffectToolPos.x, lastEffectToolPos.y);
		PackDWORDs(regKey, L"lastEffectToolSize", lastEffectToolSize.x, lastEffectToolSize.y);
		regKey.Close();
	}
	return (DWORD) lr;
}

DWORD AppSettings::Load()
{
	CRegKey regKey;
	LRESULT lr = regKey.Create(HKEY_CURRENT_USER, L"SOFTWARE\\CartelMania");
	if (lr == ERROR_SUCCESS)
	{
		regKey.QueryDWORDValue(L"debugLog", debugLog);
		UnpackDWORDs(regKey, L"lastTextEditToolPos", lastTextEditToolPos.x, lastTextEditToolPos.y);
		UnpackDWORDs(regKey, L"lastColorEditToolPos", lastColorEditToolPos.x, lastColorEditToolPos.y);
		UnpackDWORDs(regKey, L"lastShapeEditToolPos", lastShapeEditToolPos.x, lastShapeEditToolPos.y);
		UnpackDWORDs(regKey, L"lastShapeEditToolSize", lastShapeEditToolSize.x, lastShapeEditToolSize.y);
		UnpackDWORDs(regKey, L"lastLayoutEditToolPos", lastLayoutEditToolPos.x, lastLayoutEditToolPos.y);
		UnpackDWORDs(regKey, L"lastLineLayoutToolPos", lastLineLayoutToolPos.x, lastLineLayoutToolPos.y);
		UnpackDWORDs(regKey, L"lastLineLayoutToolSize", lastLineLayoutToolSize.x, lastLineLayoutToolSize.y);
		UnpackDWORDs(regKey, L"lastEffectToolPos", lastEffectToolPos.x, lastEffectToolPos.y);
		UnpackDWORDs(regKey, L"lastEffectToolSize", lastEffectToolSize.x, lastEffectToolSize.y);
		regKey.Close();
	}
	return (DWORD) lr;
}