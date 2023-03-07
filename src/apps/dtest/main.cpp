#include <iostream>

#include "base/io/log.h"
#include "dataset/dataset.h"
#include "base/conv/numtostr.h"
#include "csvloader.h"
#include "datasettools.h"

using namespace Vizzu::Dataset;

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
    datasetDump(dataset1);
    selectSeriesTypes(dataset1);
    Dataset dataset2(dataset1);
    dataset1.clear();
    dataset2.C2DConverter = [=](const MutableSeriesPtr&, double cv) -> std::string {
        Conv::NumberToString conv;
        conv.fractionDigitCount = 1;
        return conv.convert(cv);
    };
    dataset2.getMutableSeries("ENGINE SIZE")->select(ValueType::discrete);
    auto range1 = dataset2.getMutableSeries("ENGINE SIZE")->range();
    seriesRangeDump(range1);
    dataset2.C2DConverter = [=](const MutableSeriesPtr&, double cv) -> std::string {
        Conv::NumberToString conv;
        conv.fractionDigitCount = 0;
        return conv.convert(cv);
    };
    dataset2.getMutableSeries("CYLINDERS")->select(ValueType::discrete);
    auto range2 = dataset2.getMutableSeries("CYLINDERS")->range();
    seriesRangeDump(range2);
    datasetDump(dataset2);
    /*auto aggregator = std::make_shared<VizzuTableAggregator>(
        dataset2,
        Generators::RecordCounter("#"),
        DiscreteSeries("Engine size", "ENGINE SIZE"),
        DiscreteSeries("Cylinder count", "CYLINDERS"),
        Aggregators::Min("Min. fuel consumption l/100Km", "COMB (L/100 km)"),
        Aggregators::Max("Max. fuel consumption l/100Km", "COMB (L/100 km)"),
        Aggregators::Avarage("Avarage fuel consumption l/100Km", "COMB (L/100 km)"),
        Aggregators::Avarage("Avarage emission", "EMISSIONS")
    );
    dataset2.addTable("aggregated1", aggregator);*/
    return 0;
}
