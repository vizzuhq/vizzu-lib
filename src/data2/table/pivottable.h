#ifndef PiVOTTABLE_H
#define PiVOTTABLE_H

#include "texttable.h"

namespace Vizzu
{
namespace Data
{

class PivotTable
{
public:

	explicit PivotTable(TextTable table, bool hasKeyColumn = true);
	TextTable getTable();

private:
	TextTable table;
	bool hasKeyColumn;
};

}
}

#endif
