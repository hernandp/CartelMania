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

	m_table.push_back(make_pair(L"Solid",       [] { return make_unique<TextFxSolid>(); }));
	m_table.push_back(make_pair(L"Two Outlines", [] { return make_unique<TextFxTwoOutlines>(); }));
	m_table.push_back(make_pair(L"Rear Shadow", [] { return make_unique<TextFxShadow>(ShadowType::Rear); }));
	m_table.push_back(make_pair(L"Fore Shadow", [] { return make_unique<TextFxShadow>(ShadowType::Fore); }));
}