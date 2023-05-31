#include <iostream>

#include "base/io/log.h"
#include "dataset/dataset.h"
#include "dataset/tables/recordaggregator.h"
#include "dataset/tables/filters.h"
#include "dataset/tables/sorters.h"

#include "base/conv/numtostr.h"
#include "csvloader.h"
#include "datasettools.h"

using namespace Vizzu::Dataset;

ConstantTablePtr generateAvarageConsumptionTable1(Vizzu::Dataset::Dataset& ds) {
    Conv::NumberToString conv;
    auto myConverter = [&](Dataset& ds, ValueType type, Value value) -> Value {
        if (type == ValueType::continous)
            return ds.getValue(conv(value.getc()).c_str());
        return Value{};
    };
    conv.fractionDigitCount = 1;
    ds.getSeriesAs<RawSeries>("ENGINE SIZE")->selectType(ValueType::discrete, myConverter);
    conv.fractionDigitCount = 0;
    ds.getSeriesAs<RawSeries>("CYLINDERS")->selectType(ValueType::discrete, myConverter);
    auto generator = std::make_shared<RecordAggregator>(ds);
    generator->setFilter(std::make_shared<Filters::ByRange>("YEAR", 2010, 2015));
    generator->setup(
        RecordAggregator::GeneratedSeries{"#", Generators::Ordinal{}},
        RecordAggregator::DiscreteSeries{"Engine size", "ENGINE SIZE"},
        RecordAggregator::DiscreteSeries{"Cylinder count", "CYLINDERS"},
        RecordAggregator::AggregatedSeries{"Vehicles in category", Aggregators::Count{}},
        RecordAggregator::AggregatedSeries{"Min. fuel consumption l/100Km", Aggregators::Min{"COMB (L/100 km)"}},
        RecordAggregator::AggregatedSeries{"Max. fuel consumption l/100Km", Aggregators::Max{"COMB (L/100 km)"}},
        RecordAggregator::AggregatedSeries{"Avarage fuel consumption l/100Km", Aggregators::Avarage{"COMB (L/100 km)"}},
        RecordAggregator::AggregatedSeries{"Avarage emission", Aggregators::Avarage{"EMISSIONS"}}
    );
    auto table = ds.newTable<GeneratedTable>("avarage_cons", generator);
    table->setFilter(std::make_shared<Filters::ByRange>("Vehicles in category", 30, 10000));
    table->refresh();
    ds.getSeriesAs<RawSeries>("Cylinder count")->selectType(ValueType::continous);
    ds.getSeriesAs<RawSeries>("Engine size")->selectType(ValueType::continous);
    table->setSorter(std::make_shared<Sorters::MultiColumn>("Cylinder count", "Engine size"));
    return table;
}

ConstantTablePtr generateAvarageConsumptionTable2(Vizzu::Dataset::Dataset& ds) {
    auto generator = std::make_shared<RecordAggregator>(ds);
    generator->setup(
        RecordAggregator::DiscreteSeries{"cylinders", "T(avarage_cons)Cylinder count"},
        RecordAggregator::AggregatedSeries{"emission", Aggregators::Avarage{"T(avarage_cons)Avarage emission"}}
    );
    auto table = ds.newTable<GeneratedTable>("avarage_by_cylinders", generator);
    table->setSorter(std::make_shared<Sorters::MultiColumn>("cylinders"));
    table->refresh();
    table->addParam(Vizzu::Dataset::ParamNames::disp_name, "Avarage emission by cilynder count");
    table->getSeries("cylinders")->addParam(Vizzu::Dataset::ParamNames::disp_name, "Cilynder count");
    table->getSeries("emission")->addParam(Vizzu::Dataset::ParamNames::disp_name, "Avarage emission");
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
    auto table1 = generateAvarageConsumptionTable1(dataset2);
    tableToCSV("test1", table1);
    auto table2 = generateAvarageConsumptionTable2(dataset2);
    tableToCSV("test2", table2);
    return 0;
}
