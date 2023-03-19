#ifndef DATASET_SORTERS_H
#define DATASET_SORTERS_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu {
namespace Dataset {
namespace Sorters {

class MultiColumn : public AbstractSorter {
public:
    using series_info = std::vector<std::pair<std::string, ConstantSeriesPtr>>;

    class RecordValueComparator {
    public:
        const series_info& seriesInfo;

        RecordValueComparator(const series_info& si) :
            seriesInfo(si)
        {
        }
        bool operator()(int a, int b) const;
    };

    using sorter_map = std::multimap<int, int, RecordValueComparator>;
    using sorter_iterator = sorter_map::const_iterator;

public:
    class MapIterator : public AbstractSorter::Iterator {
    friend class MultiColumn;
    public:
        sorter_iterator mapIter;
        sorter_iterator mapIterEnd;

        bool test() override;
        void next() override;
        int index() const override;
    };

public:
    MultiColumn() :
        sortedValues(RecordValueComparator{seriesInfo})
    {
        endIterator.mapIterEnd = sortedValues.end();
    }

    template<typename... Args>
    MultiColumn(const char* name, Args... args) :
        MultiColumn(args...)
    {
        seriesInfo.insert(seriesInfo.begin(), std::make_pair(name, ConstantSeriesPtr{}));
    }

    void setup(const SeriesContainer&) override;
    void sortRecord(int) override;
    iterator_ptr result() override;

protected:
    series_info seriesInfo;
    sorter_map sortedValues;
    MapIterator endIterator;
};

}
}
}

#endif // DATASET_SORTERS_H
