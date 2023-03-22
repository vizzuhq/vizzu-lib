#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

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
    ds.enumSeriesAs<RawSeries>([&](const RawSeries& inst) {
        cout << "Series '" << inst.name() << "' [" << inst.size() << "] ";
        cout << "D-" << inst.typeRate(ValueType::discrete) << " C-" << inst.typeRate(ValueType::continous) << ": ";
        for(int i = 0; i < 10; i++) {
            if (inst.typeAt(i) == ValueType::continous)
                cout << "C-" << inst.valueAt(i).getc() << " ";
            if (inst.typeAt(i) == ValueType::discrete)
                cout << "D-'" << inst.valueAt(i).getd().value() << "' ";
        }
        cout << "..." << endl;
    });
}

void tableDump(const Vizzu::Dataset::ConstantTablePtr&) {
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
    list<VolatileSeriesPtr> serieses;
    for(auto name : table.series) {
        auto mds = dataset.newSeries<RawSeries>(name.c_str());
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

void tableToCSV(const char* file, const Vizzu::Dataset::ConstantTablePtr& table) {
    ofstream output(file);
    for(auto row : table->rows()) {
        bool firstValue = true;
        for(const auto& cell : row) {
            if (!firstValue)
                output << ",";
            firstValue = false;
            if (cell.type() == ValueType::continous)
                output << cell.value().getc();
            if (cell.type() == ValueType::discrete)
                output << cell.value().getd().value();
        }
        output << endl;
        output.flush();
    }
    output.close();
}

void consolidateRecordTypes(Vizzu::Dataset::Dataset& dataset) {
    dataset.enumSeriesAs<RawSeries>([&](RawSeries& inst) {
        auto rate = inst.typeRate(ValueType::continous);
        if (rate >= 0.5)
            inst.selectType(ValueType::continous);
        else
            inst.selectType(ValueType::discrete);
    });
}
