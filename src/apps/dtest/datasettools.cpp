#include <iostream>

#include "csvloader.h"
#include "dataset/dataset.h"

using namespace std;
using namespace Vizzu::DataSet;

void datasetDump(const DataSet& ds) {
    int count = 0;
    cout << "Discrete values [" << ds.values().size() << "]: ";
    for(const auto& dval : ds.values()) {
        cout << "'" << dval.value() << "'=" << dval.hash() << " ";
        if (count++ > 10)
            break;
    }
    cout << "..." << endl;
    count = 0;
    for(const auto& mds : ds.mutableSeries()) {
        auto inst = std::dynamic_pointer_cast<MutableSeries>(mds.second);
        cout << "Series '" << mds.second->name() << "' [" << mds.second->size() << "] ";
        cout << "D-" << inst->typeRate(ValueType::discrete) << " C-" << inst->typeRate(ValueType::continous) << ": ";
        for(int i = 0; i < 10; i++) {
            if (inst->typeAt(i) == ValueType::continous)
                cout << "C-" << inst->at(i).getc() << " ";
            if (inst->typeAt(i) == ValueType::discrete)
                cout << "D-'" << inst->at(i).getd().value() << "' ";
        }
        cout << "..." << endl;
    }
}

void datasetFromCSV(const CSVTable& table, DataSet& dataset) {
    list<MutableSeriesPtr> serieses;
    for(auto name : table.series) {
        auto mds = dataset.makeMutableSeries(name.c_str());
        mds->extend(table.data.size());
        serieses.push_back(mds);
    }
    for(auto record : table.data) {
        auto series = serieses.begin();
        for(auto item : record) {
            const auto p1 = std::get_if<int>(&item);
            if (p1)
                (*series)->insert(*p1);
            else {
                const auto p2 = std::get_if<double>(&item);
                if (p2)
                    (*series)->insert(*p2);
                else {
                    const auto p3 = std::get_if<string>(&item);
                    if (p3)
                        (*series)->insert(p3->c_str());
                }
            }
            series++;
        }
    }
}

void normalizeSeriesValues(Vizzu::DataSet::DataSet& dataset) {
    dataset.C2DNormalizer = [=](const MutableSeriesPtr&, double cv) -> std::string {
        return std::to_string(cv);
    };
    dataset.D2CNormalizer = [=](const MutableSeriesPtr&, const char*) -> double {
        return 0;
    };
    for(const auto& mds : dataset.mutableSeries()) {
        auto inst = std::dynamic_pointer_cast<MutableSeries>(mds.second);
        auto rate = inst->typeRate(ValueType::continous);
        if (rate >= 0.5)
            inst->normalize(ValueType::continous);
        else
            inst->normalize(ValueType::discrete);
    }
}