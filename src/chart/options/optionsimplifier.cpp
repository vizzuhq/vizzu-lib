#include "optionsimplifier.h"

#include "data2/datacube/datastat.h"
#include "data2/table/datatable.h"

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
			options.getScales().visitAll([=](ScaleId id, const Scale &scale)
			{
				if (scale.isSeriesUsed(index))
					setter->deleteSeries(id, index);
			});
		}
	}
}
