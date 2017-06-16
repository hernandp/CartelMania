#pragma once
#include "NamedTable.h"
#include "TextFx.h"

class EffectTable : public NamedTable<EffectTable, std::unique_ptr<TextFx>>
{
public:
	void Init();
};
