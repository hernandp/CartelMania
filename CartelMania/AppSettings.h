#pragma once

// Global program configuration
//

const int DEFAULT_PATH_SUBDIV_INTERVAL = 16;
const int DEFAULT_MAX_TEXT_LENGTH = 16;

struct AppSettings
{
public:
	DWORD Save();
	DWORD Load();

	bool disableSubdiv;
	bool debugDrawVertices = false;
	bool debugDisableFillPath = false;
	bool debugDrawBoundingRects = false;
	int pathSubDivInterval = DEFAULT_PATH_SUBDIV_INTERVAL;
	int maxTextLength = DEFAULT_MAX_TEXT_LENGTH;
	POINT lastTextEditToolPos = { -1,-1 };
	POINT lastColorEditToolPos = { -1,-1 };
};
