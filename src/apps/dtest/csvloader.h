#ifndef DTEST_CSVTABLE_H
#define DTEST_CSVTABLE_H

#include <list>
#include <string>
#include <variant>

class CSVTable {
public:
    using series_type = std::list<std::string>;
    using value_type = std::variant<int, double, std::string>;
    using record_type = std::list<value_type>;
    using data_type = std::list<record_type>;

    series_type series;
    data_type data;

    int load(const char* file);

protected:
    std::string trim(const std::string& str);
};

#endif //DTEST_CSVTABLE_H