#include "optionsimplifier.h"

#include "data/datacube/datastat.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionSimplifier::OptionSimplifier(OptionsSetterPtr setter, const Data::DataTable &table)
	: setter(std::move(setter)),
	  table(table)
{}

void OptionSimplifier::removeNotUsedSeries()
{
	const auto &options = setter->getOptions();
	Data::DataStat stat(table,
					  options.getScales().getDataCubeOptions(),
					  options.dataFilter.get());
	auto dimensions = options.getScales().getDimensions();
	for (const auto &index : dimensions)
	{
		if (stat.usedValueCntOf(index) < 2)
		{
			options.getScales().visitAll([=](Scales::Id id, const Scale &scale)
			{
				if (scale.isSeriesUsed(index))
					setter->deleteSeries(id, index);
			});
		}
	}
}
