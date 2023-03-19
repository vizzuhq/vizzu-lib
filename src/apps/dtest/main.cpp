#include <iostream>

#include "base/io/log.h"
#include "dataset/dataset.h"
#include "dataset/recordaggregator.h"
#include "dataset/tablefilters.h"
#include "dataset/tablesorters.h"

#include "base/conv/numtostr.h"
#include "csvloader.h"
#include "datasettools.h"

using namespace Vizzu::Dataset;

ConstantTablePtr generateAvarageConsumptionTable(Vizzu::Dataset::Dataset& ds) {
    ds.C2DConverter = [=](const AbstractConstantSeries&, double cv) -> std::string {
        Conv::NumberToString conv;
        conv.fractionDigitCount = 1;
        return conv.convert(cv);
    };
    ds.getSeriesAs<OriginalSeries>("ENGINE SIZE")->selectType(ValueType::discrete);
    ds.C2DConverter = [=](const AbstractConstantSeries&, double cv) -> std::string {
        Conv::NumberToString conv;
        conv.fractionDigitCount = 0;
        return conv.convert(cv);
    };
    ds.getSeriesAs<OriginalSeries>("CYLINDERS")->selectType(ValueType::discrete);
    auto generator = std::make_shared<RecordAggregator>(ds);
    generator->setFilter(std::make_shared<Filters::ByRange>("YEAR", 2010, 2015));
    generator->setup(
        RecordAggregator::SeriesMarker{"#", Generators::Ordinal{}},
        RecordAggregator::SeriesMarker{"Engine size", "ENGINE SIZE"},
        RecordAggregator::SeriesMarker{"Cylinder count", "CYLINDERS"},
        RecordAggregator::SeriesMarker{"Vehicles in category", Aggregators::Count{}},
        RecordAggregator::SeriesMarker{"Min. fuel consumption l/100Km", Aggregators::Min{"COMB (L/100 km)"}},
        RecordAggregator::SeriesMarker{"Max. fuel consumption l/100Km", Aggregators::Max{"COMB (L/100 km)"}},
        RecordAggregator::SeriesMarker{"Avarage fuel consumption l/100Km", Aggregators::Avarage{"COMB (L/100 km)"}},
        RecordAggregator::SeriesMarker{"Avarage emission", Aggregators::Avarage{"EMISSIONS"}}
    );
    auto table = ds.newTable<GeneratedTable>("avarage_cons", generator);
    table->setFilter(std::make_shared<Filters::ByRange>("Vehicles in category", 30, 10000));
    table->setSorter(std::make_shared<Sorters::MultiColumn>("Cylinder count", "Avarage emission"));
    table->refresh();
    return table;
}

int main(int argc, char *argv[]) {
    std::cout << "Dataset test suit v0.1" << std::endl;
    if (argc != 2) {
        std::cout << "usage: dtest <csv file name>" << std::endl;
        return 0;
    }
    Dataset dataset1;
    datasetFromCSV(argv[1], dataset1);
    consolidateRecordTypes(dataset1);
    datasetDump(dataset1);
    Dataset dataset2(dataset1);
    datasetDump(dataset2);
    auto table1 = generateAvarageConsumptionTable(dataset2);
    tableToCSV("test", table1);
    return 0;
}
