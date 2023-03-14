#include <map>

#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "originalseries.h"
#include "range.h"

namespace Vizzu {
namespace Dataset {

Range::index_iter::index_iter()
    : owner(nullptr), headPos(endpos), tailPos(endpos)
{
}

Range::index_iter::index_iter(const Range* owner, int head, int tail)
    : owner(owner), headPos(head), tailPos(tail)
{
}

Range::index_iter& Range::index_iter::operator++() {
    if (tailPos == endpos)
        tailPos = inhead;
    else if (tailPos == inhead)
        tailPos = owner->headValues[headPos].nextItem;
    else {
        tailPos = owner->tailValues[tailPos].nextItem;
        if (tailPos == inhead)
            tailPos = endpos;
    }
    return *this;
}

Range::index_iter Range::index_iter::operator++(int) {
    index_iter temp(*this);
    operator++();
    return temp;
}

bool Range::index_iter::operator==(const index_iter& arg) const {
    if (tailPos == endpos)
        return tailPos == arg.tailPos;
    return owner == arg.owner && headPos == arg.headPos && tailPos == arg.tailPos;
}

bool Range::index_iter::operator!=(const index_iter& arg) const {
    return !operator==(arg);
}

int Range::index_iter::operator*() const {
    if (tailPos == inhead)
        return owner->headValues[headPos].seriesIndex;
    else
        return owner->tailValues[tailPos].seriesIndex;
}

Range::Range(const ConstantSeriesPtr& series)
    : valueSet(series)
{
    generate();
}

ConstantSeriesPtr Range::series() const {
    return valueSet;
}

int Range::size() const {
    return headValues.size();
}

ValueType Range::typeAt(int index) const {
    return valueSet->typeAt(headValues[index].seriesIndex);
}

Value Range::valueAt(int index) const {
    return valueSet->valueAt(headValues[index].seriesIndex);
}

Range::value_iter Range::begin() const {
    return value_iter{0, this};
}

Range::value_iter Range::end() const {
    return value_iter{size(), this};
}

Range::index_iter Range::indices_begin(value_iter iter) const {
    return index_iter{this, (int)iter, index_iter::inhead};
}

Range::index_iter Range::indices_end(value_iter iter) const {
    return index_iter{this, (int)iter, index_iter::endpos};
}

void Range::generate() {
    struct Item {
        int firstItem;
        int lastItem;
    };
    struct SeriesIndexComparator {
        SeriesIndexComparator(const ConstantSeriesPtr& series) : sptr(series) {}
        bool operator()(int a, int b) const {
            if (sptr->typeAt(a) != sptr->typeAt(b))
                throw dataset_error("values are not comparable");
            if (sptr->typeAt(a) == ValueType::continous)
                return sptr->valueAt(a).getc() < sptr->valueAt(b).getc();
            if (sptr->typeAt(a) == ValueType::discrete)
                return sptr->valueAt(a).getd().hash() < sptr->valueAt(b).getd().hash();
            throw dataset_error("values are not set");
        }
        const ConstantSeriesPtr& sptr;
    };
    headValues.reserve(valueSet->size());
    tailValues.reserve(valueSet->size());
    std::map<int, Item, SeriesIndexComparator> parameters(SeriesIndexComparator{valueSet});
    for(int i = 0; i < valueSet->size(); i++) {
        auto iter = parameters.find(i);
        if (iter == parameters.end()) {
            int pos = (int)headValues.size();
            headValues.push_back(IndexItem{i, -1});
            parameters.insert(std::make_pair(i, Item{pos, -1}));
        }
        else {
            int pos = (int)tailValues.size();
            tailValues.push_back(IndexItem{i, -1});
            if (iter->second.lastItem == -1)
                headValues[iter->second.firstItem].nextItem = pos;
            else
                tailValues[iter->second.lastItem].nextItem = pos;
            iter->second.lastItem = pos;
        }
    }
    headValues.shrink_to_fit();
    tailValues.shrink_to_fit();
}

}
}
