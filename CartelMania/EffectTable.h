#pragma once
#include "NamedTable.h"
#include "TextFx.h"
#include <functional>
#include <memory>

using EffectFactoryMethod_T = std::function<std::unique_ptr<TextFx>()>;

class EffectTable : public NamedTable<EffectTable, EffectFactoryMethod_T>
{
public:
	void Init();
};
