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

	m_table.push_back(make_pair(L"Solid", make_unique<TextFxSolid>()));
	m_table.push_back(make_pair(L"Rear Shadow", make_unique<TextFxShadow>(ShadowType::Rear)));
	m_table.push_back(make_pair(L"Fore Shadow", make_unique<TextFxShadow>(ShadowType::Fore)));
}