#include "dataset.h"
#include "series.h"
#include "value.h"
#include "iterators.h"
#include "mutableseries.h"
#include "recordaggregator.h"

namespace Vizzu {
namespace Dataset {

RecordAggregator::GeneratedSeries::GeneratedSeries(const char*, const AbstractSeriesGenerator&) {
}

RecordAggregator::DiscreteSeries::DiscreteSeries(const char*, const char*) {
}

RecordAggregator::AggregatedSeries::AggregatedSeries(const char*, const AbstractSeriesAggregator&) {
}

RecordAggregator::RecordAggregator(Dataset& dataset)
    : dataset(dataset)
{
}

}
}
