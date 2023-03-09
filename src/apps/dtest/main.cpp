#include <iostream>

#include "base/io/log.h"
#include "dataset/dataset.h"
#include "dataset/recordaggregator.h"

#include "base/conv/numtostr.h"
#include "csvloader.h"
#include "datasettools.h"

using namespace Vizzu::Dataset;

void generateAvarageConsumptionTable(Vizzu::Dataset::Dataset& ds) {
    ds.C2DConverter = [=](const MutableSeriesPtr&, double cv) -> std::string {
        Conv::NumberToString conv;
        conv.fractionDigitCount = 1;
        return conv.convert(cv);
    };
    ds.getMutableSeries("ENGINE SIZE")->select(ValueType::discrete);
    ds.getMutableSeries("ENGINE SIZE")->range();
    ds.C2DConverter = [=](const MutableSeriesPtr&, double cv) -> std::string {
        Conv::NumberToString conv;
        conv.fractionDigitCount = 0;
        return conv.convert(cv);
    };
    ds.getMutableSeries("CYLINDERS")->select(ValueType::discrete);
    ds.getMutableSeries("CYLINDERS")->range();
    auto generator = std::make_shared<RecordAggregator>(ds);
    generator->setup(
        RecordAggregator::GeneratedSeries("#", Generators::Ordinal{}),
        RecordAggregator::DiscreteSeries("Engine size", "ENGINE SIZE"),
        RecordAggregator::DiscreteSeries("Cylinder count", "CYLINDERS"),
        RecordAggregator::AggregatedSeries("Vehicle count in category", Aggregators::Count{}),
        RecordAggregator::AggregatedSeries("Min. fuel consumption l/100Km", Aggregators::Min{"COMB (L/100 km)"}),
        RecordAggregator::AggregatedSeries("Max. fuel consumption l/100Km", Aggregators::Max{"COMB (L/100 km)"}),
        RecordAggregator::AggregatedSeries("Avarage fuel consumption l/100Km", Aggregators::Avarage{"COMB (L/100 km)"}),
        RecordAggregator::AggregatedSeries("Avarage emission", Aggregators::Avarage{"EMISSIONS"})
    );
    ds.addTable("avarage_consumption", generator);
}

int main(int argc, char *argv[]) {
    std::cout << "Dataset test suit v0.1" << std::endl;
    if (argc != 2) {
        std::cout << "usage: dtest <csv file name>" << std::endl;
        return 0;
    }
    CSVTable csv;
    if (csv.load(argv[1]) <= 0) {
        std::cout << "Failed to open file or file is empty." << std::endl;
        return -1;
    }
    Dataset dataset1;
    datasetFromCSV(csv, dataset1);
    unifySeriesTypes(dataset1);
    datasetDump(dataset1);
    Dataset dataset2(dataset1);
    generateAvarageConsumptionTable(dataset2);
    datasetDump(dataset2);
    return 0;
}
