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
#include <windows.h>
#include <strsafe.h>

#define XASSERT(_cond) do { if ( (_cond) == false) { _FatalMsg("ASSERT-FAIL:\n(%s:%d):\n%s\n\nLast Win32 error code is %d. Thread ID: 0x%x", __FILE__, __LINE__, #_cond, GetLastError(), GetCurrentThreadId() ); } } while(0);

inline void _FatalMsg(LPCSTR fmt, ...)
{
	const int MAX_MESSAGE = 1024;
	va_list args;
	va_start(args, fmt);

	char szBuffer[MAX_MESSAGE] = { 0 };
	StringCchVPrintfA(szBuffer, MAX_MESSAGE, fmt, args);
	va_end(args);

	MessageBoxA(0, szBuffer, "Fatal Error", MB_ICONERROR | MB_OK);

	DebugBreak();
}

inline void dprintf(LPCWSTR fmt, ...)
{
#ifdef _DEBUG
	const int MAX_MESSAGE = 1024;
	va_list args;
	va_start(args, fmt);

	WCHAR szBuffer[MAX_MESSAGE] = { 0 };
	StringCchVPrintfW(szBuffer, MAX_MESSAGE, fmt, args);
	va_end(args);

	OutputDebugStringW(szBuffer);
#endif
}
