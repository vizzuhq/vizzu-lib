#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "recordaggregator.h"

namespace Vizzu {
namespace Dataset {

RecordAggregator::RecordAggregator(Dataset& dataset)
    : dataset(dataset)
{
}

void RecordAggregator::generate() {
    output->prepare(markers.size());
    aggregator_vector aggregators;
    int rawRecordCount = processMarkers(aggregators);
    auto records = collectRecords(rawRecordCount, aggregators);
    generateRecords(records, aggregators);
}

int RecordAggregator::processMarkers(aggregator_vector& aggregators) {
    int recordCount = 0;
    for(auto& marker : markers) {
        if (!marker.discreteSeriesName.empty()) {
            auto dsPtr = dataset.getSeries(marker.discreteSeriesName.c_str());
            if (!dsPtr)
                throw dataset_error("unknown discrete series");
            if (recordCount == 0)
                recordCount = dsPtr->size();
            else if (recordCount != dsPtr->size())
                throw dataset_error("discrete series length mismatch");
            marker.tableSeries = std::make_shared<OriginalSeries>(dataset);
            marker.tableSeries->selectType(ValueType::discrete);
            output->insert(marker.tableSeries);
            auto hash = DiscreteValue::hash(dsPtr->name());
            aggregators.push_back(AggregatorItem{hash, dsPtr});
            marker.aggregatorIndex = aggregators.size() - 1;
        }
        else if (marker.generator != nullptr) {
            marker.tableSeries = std::make_shared<OriginalSeries>(dataset);
            marker.tableSeries->selectType(marker.generator->type());
            output->insert(marker.tableSeries);
            marker.aggregatorIndex = -1;
        }
        else if (marker.aggregator != nullptr) {
            marker.tableSeries = std::make_shared<OriginalSeries>(dataset);
            marker.tableSeries->selectType(marker.aggregator->type());
            output->insert(marker.tableSeries);
            marker.aggregatorIndex = -1;
        }
    }
    return recordCount;
}

RangePtr RecordAggregator::collectRecords(int count, const aggregator_vector& aggregators) {
    auto hashSeries = std::make_shared<OriginalSeries>(dataset);
    hashSeries->selectType(ValueType::continous);
    hashSeries->extend(count);
    for(int index = 0; index < count; index++) {
        double recordHash = 1;
        for(const auto& item : aggregators)
            recordHash *= item.hash * item.series->valueAt(index).getd().hash();
        hashSeries->insert(recordHash);
    }
    return std::make_shared<Range>(hashSeries);
}

void RecordAggregator::generateRecords(const RangePtr& range, const aggregator_vector& aggregators) {
    for(auto viter = range->begin(); viter != range->end(); viter++) {
        auto from = range->indices_begin(viter);
        auto to = range->indices_end(viter);
        for(auto& marker : markers) {
            if (marker.aggregatorIndex != -1) {
                auto value = aggregators[marker.aggregatorIndex].series->valueAt(*from);
                marker.tableSeries->insert(ValueType::discrete, value);
            }
            if (marker.aggregator != nullptr)
                marker.tableSeries->insert(marker.aggregator->type(), marker.aggregator->calculate(from, to));
            else if (marker.generator != nullptr)
                marker.tableSeries->insert(marker.generator->type(), marker.generator->calculate());
        }
    }
}

void RecordAggregator::setOutput(const AbstractTableGenerator::output_table_ptr& output) {
    this->output = output;
}

void RecordAggregator::setFilter(const FilterPtr &) {
}

}
}
