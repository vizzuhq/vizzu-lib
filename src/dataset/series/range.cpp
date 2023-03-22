#include <map>

#include "../dataset.h"
#include "value.h"
#include "iterators.h"
#include "rawseries.h"
#include "range.h"

namespace Vizzu {
namespace Dataset {

RangeIndexIterator::RangeIndexIterator()
    : owner(nullptr), headPos(endpos), tailPos(endpos)
{
}

RangeIndexIterator::RangeIndexIterator(const Range* owner, int head, int tail)
    : owner(owner), headPos(head), tailPos(tail)
{
}

RangeIndexIterator& RangeIndexIterator::operator++() {
    if (tailPos == endpos)
        tailPos = inhead;
    else if (tailPos == inhead) {
        tailPos = owner->headValues[headPos].nextItem;
        if (tailPos == inhead)
            tailPos = endpos;
    }
    else {
        tailPos = owner->tailValues[tailPos].nextItem;
        if (tailPos == inhead)
            tailPos = endpos;
    }
    return *this;
}

RangeIndexIterator RangeIndexIterator::operator++(int) {
    RangeIndexIterator temp(*this);
    operator++();
    return temp;
}

bool RangeIndexIterator::operator==(const RangeIndexIterator& arg) const {
    if (tailPos == endpos)
        return tailPos == arg.tailPos;
    return owner == arg.owner && headPos == arg.headPos && tailPos == arg.tailPos;
}

bool RangeIndexIterator::operator!=(const RangeIndexIterator& arg) const {
    return !operator==(arg);
}

int RangeIndexIterator::operator*() const {
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

RangeValueIterator Range::begin() const {
    return RangeValueIterator{0, this};
}

RangeValueIterator Range::end() const {
    return RangeValueIterator{size(), this};
}

RangeIndexIterator Range::indices_begin(RangeValueIterator iter) const {
    return RangeIndexIterator{this, (int)iter, RangeIndexIterator::inhead};
}

RangeIndexIterator Range::indices_end(RangeValueIterator iter) const {
    return RangeIndexIterator{this, (int)iter, RangeIndexIterator::endpos};
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
