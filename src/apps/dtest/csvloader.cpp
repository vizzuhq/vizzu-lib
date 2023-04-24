#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include "csvloader.h"

using namespace std;

string CSVTable::trim(const string& str) {
    const string& chars = " \t\r\n";
    auto begin = str.find_first_not_of(chars);
    if (begin == string::npos)
        return "";
    auto end = str.find_last_not_of(chars);
    return str.substr(begin, end - begin + 1);
}

int CSVTable::load(const char* file) {
    ifstream input(file);
    if (!input)
        return -1;
    string line;
    if (getline(input, line)) {
        stringstream ss(line);
        string field;
        while (getline(ss, field, ','))
            series.push_back(trim(field));
    } else
        return -1;
    while (getline(input, line)) {
        record_type record;
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
    return data.size();
}
