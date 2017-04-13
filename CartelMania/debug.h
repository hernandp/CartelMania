#pragma once
#include <windows.h>
#include <strsafe.h>
#define XASSERT(_cond) do { if ( (_cond) == false) { _FatalMsg("ASSERT-FAIL:\n(%s:%d):\n%s", __FILE__, __LINE__, #_cond ); } } while(0);

inline void _FatalMsg(LPCSTR fmt, ...)
{
	const int MAX_MESSAGE = 1024;
	va_list args;
	va_start(args, fmt);

	char szBuffer[MAX_MESSAGE] = { 0 };
	StringCchVPrintfA(szBuffer, MAX_MESSAGE, fmt, args);
	va_end(args);

	MessageBoxA(0, szBuffer, "Fatal Error", MB_ICONERROR | MB_OK);
}

