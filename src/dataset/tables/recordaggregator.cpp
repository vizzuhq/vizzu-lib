#include "../dataset.h"
#include "../series/value.h"
#include "../series/iterators.h"
#include "recordaggregator.h"

namespace Vizzu {
namespace Dataset {

RecordAggregator::RecordAggregator(Dataset& dataset)
    : dataset(dataset)
{
}

void RecordAggregator::generate() {
    table->prepare(markers.size());
    index_vector inputMarkers;
    index_vector outputMarkers;
    int rawRecordCount = processMarkers(inputMarkers, outputMarkers);
    for(auto index : outputMarkers) {
        if (markers[index].aggregator)
            markers[index].aggregator->setup(dataset);
        else if (markers[index].generator)
            markers[index].generator->setup(dataset);
    }
    if (filter)
        rawRecordCount = filterRecords(rawRecordCount, inputMarkers);
    auto records = collectRecords(rawRecordCount, inputMarkers);
    for(auto& marker : markers) {
        marker.resultSeries->extend(records->size());
    }
    generateRecords(records, inputMarkers, outputMarkers);
    table->finalize();
}

int RecordAggregator::processMarkers(index_vector& input, index_vector& output) {
    int recordCount = 0;
    int columnCount = 0;
    for(auto& marker : markers) {
        if (!marker.aggregatorName.empty()) {
            marker.aggregatorSeries = dataset.getSeries(marker.aggregatorName.c_str());
            if (!marker.aggregatorSeries)
                throw dataset_error("unknown discrete series");
            if (recordCount == 0)
                recordCount = marker.aggregatorSeries->size();
            else if (recordCount != marker.aggregatorSeries->size())
                throw dataset_error("discrete series length mismatch");
            marker.resultSeries = dataset.newSeries<RawSeries>(marker.resultName.c_str());
            marker.resultSeries->selectType(ValueType::discrete);
            marker.aggregatorHash = DiscreteValue::hash(marker.aggregatorSeries->name());
            table->insert(marker.resultSeries);
            input.push_back(columnCount);
        }
        else if (marker.generator != nullptr) {
            marker.resultSeries = dataset.newSeries<RawSeries>(marker.resultName.c_str());
            marker.resultSeries->selectType(marker.generator->type());
            table->insert(marker.resultSeries);
            output.push_back(columnCount);
        }
        else if (marker.aggregator != nullptr) {
            marker.resultSeries = dataset.newSeries<RawSeries>(marker.resultName.c_str());
            marker.resultSeries->selectType(marker.aggregator->type());
            table->insert(marker.resultSeries);
            output.push_back(columnCount);
        }
        columnCount++;
    }
    return recordCount;
}

int RecordAggregator::filterRecords(int recordCount, const index_vector& input) {
    LinkedSeriesPtr link;
    filter->setup(dataset);
    for(auto index : input) {
        auto aptr = markers[index].aggregatorSeries;
        auto lptr = std::make_shared<LinkedSeries>(aptr);
        if (!filteredIndeces) {
            filteredIndeces = lptr->createSelector(recordCount);
            link = lptr;
        }
        else
            lptr->setSelector(filteredIndeces);
        markers[index].aggregatorSeries = lptr;
    }
    for(int ai = 0; ai < recordCount; ai++) {
        if (filter->filterRecord(ai))
            link->select(ai);
    }
    filteredIndeces->shrink_to_fit();
    return filteredIndeces->size();
}

RangePtr RecordAggregator::collectRecords(int count, const index_vector& input) {
    auto hashSeries = std::make_shared<RawSeries>(dataset);
    hashSeries->selectType(ValueType::continous);
    hashSeries->extend(count);
    for(int ai = 0; ai < count; ai++) {
        double recordHash = 1;
        for(const auto& mi : input)
            recordHash *= markers[mi].aggregatorHash * markers[mi].aggregatorSeries->valueAt(ai).getd().hash();
        hashSeries->insert(recordHash);
    }
    return std::make_shared<Range>(hashSeries);
}

void RecordAggregator::generateRecords(const RangePtr& range, const index_vector& input, const index_vector& output) {
    for(auto viter = range->begin(); viter != range->end(); viter++) {
        auto from = range->indices_begin(viter);
        auto to = range->indices_end(viter);
        for(auto index : input) {
            auto value = markers[index].aggregatorSeries->valueAt(*from);
            markers[index].resultSeries->insert(ValueType::discrete, value);            
        }
        for(auto iter = from; iter != to; iter++) {
            for(auto index : output) {
                if (markers[index].aggregator)
                    markers[index].aggregator->aggregateRecord(*iter);
            }
        }
        for(auto index : output) {
            auto& series = markers[index].resultSeries;
            if (markers[index].aggregator)
                series->insert(markers[index].aggregator->type(), markers[index].aggregator->aggregatedValue());
            else if (markers[index].generator)
                series->insert(markers[index].generator->type(), markers[index].generator->generateRecord((int)viter));
        }
    }
}

void RecordAggregator::setOutput(const AbstractTableGenerator::output_table_ptr& output) {
    table = output;
}

void RecordAggregator::setFilter(const FilterPtr& fptr) {
    filter = fptr;
}

}
}
