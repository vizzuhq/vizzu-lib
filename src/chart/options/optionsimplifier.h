#ifndef OPTIONSIMPLIFIER_H
#define OPTIONSIMPLIFIER_H

#include "optionssetter.h"

namespace Vizzu
{

namespace Data { class DataTable; }

namespace Diag
{

class OptionSimplifier
{
public:
	OptionSimplifier(OptionsSetterPtr setter, const Data::DataTable &table);
	void removeNotUsedSeries();
private:
	OptionsSetterPtr setter;
	const Data::DataTable &table;
};

}
}

#endif
