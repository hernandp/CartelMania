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

private:
	LSTATUS PackDWORDs(CRegKey& k, const wchar_t* valueName, DWORD dwHi, DWORD dwLo);

	template <class T>
	LSTATUS UnpackDWORDs(CRegKey& k, const wchar_t* valueName, T& dwHi, T& dwLo);
};
