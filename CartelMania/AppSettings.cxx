#include "stdafx.h"
#include "AppSettings.h"

DWORD AppSettings::Save()
{
	CRegKey regKey;
	LRESULT lr = regKey.Create(HKEY_CURRENT_USER, L"SOFTWARE\\CartelMania");
	if (lr == ERROR_SUCCESS)
	{
		regKey.SetQWORDValue(L"lastTextEditToolPos", ((UINT64) lastTextEditToolPos.x << 32) | lastTextEditToolPos.y);
		regKey.SetQWORDValue(L"lastColorEditToolPos", ((UINT64) lastColorEditToolPos.x << 32) | lastColorEditToolPos.y);
		regKey.Close();
	}
	return lr;
}

DWORD AppSettings::Load()
{
	CRegKey regKey;
	LRESULT lr = regKey.Create(HKEY_CURRENT_USER, L"SOFTWARE\\CartelMania");
	if (lr == ERROR_SUCCESS)
	{
		ULONGLONG textToolPos;
		if (ERROR_SUCCESS == regKey.QueryQWORDValue(L"lastTextEditToolPos", textToolPos))
		{
			lastTextEditToolPos.x = (textToolPos >> 32) & 0x00000000ffffffff;
			lastTextEditToolPos.y = textToolPos & 0x00000000ffffffff;
		}

		ULONGLONG colorToolPos;
		if (ERROR_SUCCESS == regKey.QueryQWORDValue(L"lastColorEditToolPos", colorToolPos))
		{
			lastColorEditToolPos.x = (colorToolPos >> 32) & 0x00000000ffffffff;
			lastColorEditToolPos.y = colorToolPos & 0x00000000ffffffff;
		}
		regKey.Close();
	}
	return lr;
}