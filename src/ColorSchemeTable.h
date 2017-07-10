#include "NamedTable.h"
#include "ColorScheme.h"

class ColorSchemeTable : public NamedTable<ColorSchemeTable, ColorScheme>
{
public:
	void Init();
};
