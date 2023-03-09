#include <iostream>

#include "csvloader.h"
#include "dataset/dataset.h"

using namespace std;
using namespace Vizzu::Dataset;

void datasetDump(const Dataset& ds) {
    int count = 0;
    cout << "Discrete values [" << ds.discreteValues().size() << "]: ";
    for(const auto& dval : ds.discreteValues()) {
        cout << "'" << dval.value() << "'=" << dval.hash() << " ";
        if (count++ > 10)
            break;
    }
    cout << "..." << endl;
    count = 0;
    for(const auto& mds : ds.mutableSeries()) {
        auto inst = std::dynamic_pointer_cast<MutableSeries>(mds.second);
        cout << "Series '" << mds.second->name() << "' [" << mds.second->size() << "] ";
        cout << "D-" << inst->rate(ValueType::discrete) << " C-" << inst->rate(ValueType::continous) << ": ";
        for(int i = 0; i < 10; i++) {
            if (inst->typeAt(i) == ValueType::continous)
                cout << "C-" << inst->valueAt(i).getc() << " ";
            if (inst->typeAt(i) == ValueType::discrete)
                cout << "D-'" << inst->valueAt(i).getd().value() << "' ";
        }
        cout << "..." << endl;
    }
}

void tableDump(const Vizzu::Dataset::TablePtr&) {
}

void seriesRangeDump(const Vizzu::Dataset::RangePtr& range) {
    const auto& series = range->series();
    cout << "Series '" << series->name() << "' [" << series->size() << "]\n";
    cout << "Discrete values [" << range->size() << "] ";
    int count = 0;
    for(auto value : *range) {
        cout << "'" << value.getd().value() << "' ";
        if (count++ > 10)
            break;
    }
    cout << "\n";
    count = 0;
    for(auto viter = range->begin(); viter != range->end(); viter++) {
        cout << "indices of '" << viter.value().getd().value() << "': ";
        int count2 = 0;
        for(auto iiter = range->indices_begin(viter); iiter != range->indices_end(viter); iiter++) {
            cout << *iiter << " ";
            if (count2++ > 10)
                break;
        }
        cout << "\n";
        if (count++ > 10)
            break;
    }
    cout << "\n";
}

void datasetFromCSV(const char* fileName, Dataset& dataset) {
    CSVTable table;
    if (table.load(fileName) <= 0) {
        std::cout << "Failed to open file or file is empty." << std::endl;
        return;
    }
    list<MutableSeriesPtr> serieses;
    for(auto name : table.series) {
        auto mds = dataset.addMutableSeries(name.c_str());
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

void unifySeriesTypes(Vizzu::Dataset::Dataset& dataset) {
    for(const auto& mds : dataset.mutableSeries()) {
        auto inst = std::dynamic_pointer_cast<MutableSeries>(mds.second);
        auto rate = inst->rate(ValueType::continous);
        if (rate >= 0.5)
            inst->select(ValueType::continous);
        else
            inst->select(ValueType::discrete);
    }
}
