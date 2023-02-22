#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <variant>

#include "base/io/log.h"
#include "dataset/dataset.h"

using namespace std;
using namespace Vizzu;

using SeriesNames = list<string>;
using Value = variant<int, double, string>;
using Record = list<Value>;
using Table = list<Record>;

string trim(const string& str, const string& chars = " \t\r\n") {
    auto begin = str.find_first_not_of(chars);
    if (begin == string::npos) return "";
    auto end = str.find_last_not_of(chars);
    return str.substr(begin, end - begin + 1);
}

int csvloader(const char* file, SeriesNames& names, Table& data) {
    ifstream input(file);
    if (!input) {
        cout << "Failed to open file." << endl;
        return -1;
    }
    string line;
    if (getline(input, line)) {
        stringstream ss(line);
        string field;
        while (getline(ss, field, ','))
            names.push_back(trim(field));
    } else
        return -1;
    while (getline(input, line)) {
        Record record;
        stringstream ss(line);
        string field;
        string quotedString;
        while (getline(ss, field, ',')) {
            istringstream intStream(trim(field));
            int intValue;
            if (intStream >> intValue && intStream.eof()) {
                record.push_back(intValue);
                continue;
            }
            istringstream doubleStream(trim(field));
            double doubleValue;
            if (doubleStream >> doubleValue && doubleStream.eof()) {
                record.push_back(doubleValue);
                continue;
            }
            record.push_back(trim(field));
        }
        data.push_back(record);
    }
    input.close();
    return 0;
}

int main(int argc, char *argv[]) {
    cout << "Dataset test suit v0.1" << endl;
    std::string fileName;
    if (argc == 2)
        fileName = argv[0];
    else
        fileName = "test-data-1.csv";
    Table data;
    SeriesNames seriesNames;
    DataSet::DataSet dataset;
    if (csvloader(fileName.c_str(), seriesNames, data) >= 0) {
        list<DataSet::MutableSeriesPtr> serieses;
        for(auto name : seriesNames)
            serieses.push_back(dataset.makeMutableSeries(name.c_str()));
        for(auto record : data) {
            DataSet::Value value{DataSet::ContinousValue{}};
            for(auto item : record) {
                const auto p1 = std::get_if<int>(&item);
                if (p1)
                    value = dataset.getValue((double)*p1);
                const auto p2 = std::get_if<double>(&item);
                if (p2)
                    value = dataset.getValue(*p2);
                const auto p3 = std::get_if<string>(&item);
                if (p3)
                    value = dataset.getValue(p3->c_str());
            }
        }
    }
}
