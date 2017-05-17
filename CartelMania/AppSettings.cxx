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
		PackDWORDs(regKey, L"lastTextEditToolPos", lastTextEditToolPos.x, lastTextEditToolPos.y);
		PackDWORDs(regKey, L"lastColorEditToolPos", lastColorEditToolPos.x, lastColorEditToolPos.y);
		PackDWORDs(regKey, L"lastShapeEditToolPos", lastShapeEditToolPos.x, lastShapeEditToolPos.y);
		PackDWORDs(regKey, L"lastShapeEditToolSize", lastShapeEditToolSize.x, lastShapeEditToolSize.y);
		PackDWORDs(regKey, L"lastLayoutEditToolPos", lastLayoutEditToolPos.x, lastLayoutEditToolPos.y);
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
		UnpackDWORDs(regKey, L"lastTextEditToolPos", lastTextEditToolPos.x, lastTextEditToolPos.y);
		UnpackDWORDs(regKey, L"lastColorEditToolPos", lastColorEditToolPos.x, lastColorEditToolPos.y);
		UnpackDWORDs(regKey, L"lastShapeEditToolPos", lastShapeEditToolPos.x, lastShapeEditToolPos.y);
		UnpackDWORDs(regKey, L"lastShapeEditToolSize", lastShapeEditToolSize.x, lastShapeEditToolSize.y);
		UnpackDWORDs(regKey, L"lastLayoutEditToolPos", lastLayoutEditToolPos.x, lastLayoutEditToolPos.y);
		regKey.Close();
	}
	return (DWORD) lr;
}