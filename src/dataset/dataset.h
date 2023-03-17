#ifndef DATASET_DATASET_H
#define DATASET_DATASET_H

#include "types.h"
#include "interfaces.h"
#include "value.h"
#include "range.h"
#include "originalseries.h"
#include "iterators.h"
#include "table.h"

namespace Vizzu {
namespace Dataset {

class Dataset {
public:
    DiscreteToContinousConverter D2CConverter;
    ContinousToDiscreteConverter C2DConverter;

    Dataset();
    Dataset(const Dataset& src);

    void clear();
    bool empty();
    void compact();
    void deepCopy(const Dataset& src);
    Value getValue(double continousValue);
    Value getValue(const char* discreteValue);
    const DiscreteValueContainer& discreteValues() const;

    template<typename T, typename ...Args>
    std::shared_ptr<T> newSeries(const char* name, Args... args) {
        static int nextSeriesId = 1;
        if (seriesByName.find(name) != seriesByName.end())
            throw dataset_error("series exists with the same name");
        auto ptr = std::make_shared<T>(*this, nextSeriesId++, name, args...);
        auto cptr = std::dynamic_pointer_cast<AbstractConstantSeries>(ptr);
        seriesByName.insert(std::make_pair(cptr->name(), SeriesItem{cptr, RangePtr{}}));
        return ptr;
    }
    
    template<typename T, typename ...Args>
    std::shared_ptr<T> newTable(const char* name, Args... args) {
        static int nextTableId = 1;
        if (tablesByName.find(name) != tablesByName.end())
            throw dataset_error("table exists with the same name");
        auto ptr = std::make_shared<T>(*this, nextTableId++, name, args...);
        auto cptr = std::dynamic_pointer_cast<AbstractConstantTable>(ptr);
        tablesByName.insert(std::make_pair(cptr->name(), cptr));
        return ptr;
    }

    template<typename T>
    std::shared_ptr<T> getSeriesAs(const char* name) const {
        auto ptr = getSeries(name);
        if (ptr)
            return std::dynamic_pointer_cast<T>(ptr);
        return std::shared_ptr<T>{};
    }

    template<typename T>
    std::shared_ptr<T> getTableAs(const char*) {
        return std::shared_ptr<T>{};
    }

    template<typename T>
    void enumSeriesAs(std::function<void(T&)> callback) const {
        for(const auto& item : seriesByName) {
            auto ptr = std::dynamic_pointer_cast<T>(item.second.series);
            if (ptr)
                callback((T&)*item.second.series.get());
        }
    }

    template<typename T>
    void enumTablesAs(std::function<void(T&)>) const {
    }

    ConstantTablePtr getTable(const char* name) const;
    void deleteTable(const char* name);
    ConstantSeriesPtr getSeries(const char* name) const;
    void deleteSeries(const char* name);
    RangePtr getRange(const char* name);
    void deleteRange(const char* name);

    const TableContainer& tables() const;
    const SeriesContainer& series() const;

protected:
    TableContainer tablesByName;
    SeriesContainer seriesByName;
    DiscreteValueContainer values;
};

}
}

#endif //DATASET_DATASET_H
