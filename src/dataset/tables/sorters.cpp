#include <string.h>

#include "../dataset.h"
#include "../series/value.h"
#include "../series/iterators.h"
#include "sorters.h"

namespace Vizzu {
namespace Dataset {
namespace Sorters {

bool MultiColumn::RecordValueComparator::operator()(int a, int b) const {
    for(auto& item : seriesInfo) {
        if (item.second->type() == ValueType::continous) {
            auto va = item.second->valueAt(a).getc();
            auto vb = item.second->valueAt(b).getc();
            if (va == vb)
                continue;
            return va < vb;
        }
        else if (item.second->type() == ValueType::discrete) {
            auto va = item.second->valueAt(a).getd().value();
            auto vb = item.second->valueAt(b).getd().value();
            auto result = strcmp(va, vb);
            if (result == 0)
                continue;
            return result < 0;
        }
        else
            throw dataset_error("series type is not defined");
    }
    return false;
}

bool MultiColumn::MapIterator::test() {
    return mapIter != mapIterEnd;
}

void MultiColumn::MapIterator::next() {
    mapIter++;
}

int MultiColumn::MapIterator::index() const {
    return mapIter->first;
}

void MultiColumn::setup(const ConstantTablePtr& table) {
    for(auto& item : seriesInfo) {
        item.second = table->getSeries(item.first.c_str(), TableSeriesType::unsorted);
        if (!item.second)
            throw dataset_error("unknow series in sorter"); 
    }
}

void MultiColumn::sortRecord(int index) {
    sortedValues.insert(std::make_pair(index, index));
}

MultiColumn::iterator_ptr MultiColumn::result() {
    auto iter = std::make_shared<MapIterator>();
    iter->mapIter = sortedValues.begin();
    iter->mapIterEnd = sortedValues.end();
    return iter;
}

}
}
}
