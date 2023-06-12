#include "optionsimplifier.h"

#include "data/datacube/datastat.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionSimplifier::OptionSimplifier(OptionsSetterPtr setter,
    const Data::DataTable &table) :
    setter(std::move(setter)),
    table(table)
{}

void OptionSimplifier::removeNotUsedSeries()
{
	const auto &options = setter->getOptions();
	Data::DataStat stat(table,
	    options.getChannels().getDataCubeOptions(),
	    options.dataFilter.get());
	auto dimensions = options.getChannels().getDimensions();
	for (const auto &index : dimensions) {
		if (stat.usedValueCntOf(index) < 2) {
			options.getChannels().visitAll(
			    [=, this](ChannelId id, const Channel &channel)
			    {
				    if (channel.isSeriesUsed(index))
					    setter->deleteSeries(id, index);
			    });
		}
	}
}
